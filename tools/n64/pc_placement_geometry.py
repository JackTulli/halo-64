"""Conservative converter-only placement checks using the user's owned map.

Default-pose collision/physics body volume and nine ground support samples;
neither continuous contact nor native vehicle animation/traversal is certified.
"""
import math
import struct
from .collision import CollisionBSP

def dot(a,b):return sum(x*y for x,y in zip(a,b))
def sub(a,b):return tuple(x-y for x,y in zip(a,b))
def cross(a,b):return (a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0])
def bbox(points):return [[min(p[k] for p in points),max(p[k] for p in points)] for k in range(3)]
def corners(bounds):
    return [(x,y,z) for x in bounds[0] for y in bounds[1] for z in bounds[2]]

def object_bounds(m,t,defs):
    """Default-pose collision vertices and native physics mass spheres.

    Ignore zero cached collision bounds. Missing bind matches fail closed.
    """
    D=defs;F=lambda name:{f.name:o for o,f in D.fields(name)}
    def rows(m,base,name):
        n,p=struct.unpack("<iI",m.read(base,8));size=D.size(name)
        return [(p+i*size,m.read(p+i*size,size)) for i in range(n)]
    def ref(m,address):return m.tag(struct.unpack("<I",m.read(address+12,4))[0])
    of=F('Object');model=ref(m,t.address+of['model']);coll=ref(m,t.address+of['collision model']);physics=ref(m,t.address+of['physics'])
    nodes={};points=[];collision_vertices=0;mass_spheres=0
    if model:
        nf=F('ModelNode')
        for a,raw in rows(m,model.address+F('Model')['nodes'],'ModelNode'):
            name=raw[:32].split(b'\0')[0].decode('latin1');s=struct.unpack_from('<f',raw,nf['scale'])[0]
            r=struct.unpack_from('<9f',raw,nf['rotation']);tr=struct.unpack_from('<3f',raw,nf['translation'])
            cols=[tuple(s*x for x in r[k*3:k*3+3]) for k in range(3)];det=dot(cols[0],cross(cols[1],cols[2]))
            if not math.isfinite(det) or abs(det)<=1e-12:raise ValueError('invalid model bind transform')
            inverse_rows=[tuple(v/det for v in cross(cols[(k+1)%3],cols[(k+2)%3])) for k in range(3)]
            nodes[name]=(inverse_rows,tr)
    if coll:
        for a,raw in rows(m,coll.address+F('ModelCollisionGeometry')['nodes'],'ModelCollisionGeometryNode'):
            name=raw[:32].split(b'\0')[0].decode('latin1')
            for b,_ in rows(m,a+F('ModelCollisionGeometryNode')['bsps'],'ModelCollisionGeometryBSP'):
                bs=CollisionBSP.read(m,b)
                if not bs.vertices:continue
                if name not in nodes:raise ValueError('collision bind absent: '+name)
                inverse,tr=nodes[name]
                for v in bs.vertices:points.append(tuple(dot(r,sub(v[:3],tr)) for r in inverse));collision_vertices+=1
    if physics:
        pf=F('PhysicsMassPoint')
        for a,raw in rows(m,physics.address+F('Physics')['mass points'],'PhysicsMassPoint'):
            pos=struct.unpack_from('<3f',raw,pf['position']);radius=struct.unpack_from('<f',raw,pf['radius'])[0]
            if not math.isfinite(radius) or radius<0:raise ValueError('invalid physics mass radius')
            points.extend(tuple(pos[k]+signs[k]*radius for k in range(3)) for signs in corners([[-1,1]]*3));mass_spheres+=1
    if not points:
        radius=struct.unpack('<f',m.read(t.address+4,4))[0];offset=struct.unpack('<3f',m.read(t.address+8,12))
        if not 0<radius<5:raise ValueError('missing collision/physics and invalid object bounds')
        points=[tuple(offset[k]+s[k]*radius for k in range(3)) for s in corners([[-1,1]]*3)]
    if not all(math.isfinite(v) for p in points for v in p):raise ValueError('nonfinite object bounds')
    return {'bounds':bbox(points),'collision_vertices':collision_vertices,'mass_spheres':mass_spheres}

def empty_aabb(bs,bounds):
    """Traverse every BSP child intersecting the complete AABB, not corner rays.
    Any solid or two-sided leaf is conservative rejection/UNKNOWN. Other empty
    leaves can be crossed; no assumption that an entire box shares one leaf.
    """
    if not all(math.isfinite(v) for pair in bounds for v in pair):return False
    pending=[0];seen=0;leaves=0
    while pending:
        index=pending.pop();index=index-0x100000000 if index>=0x80000000 else index
        seen+=1
        if seen>len(bs.nodes)*2+len(bs.leaves)*2:return False
        if index<0:
            if index==-1:return False
            leaf=index&0x7fffffff
            if leaf>=len(bs.leaves) or bs.leaves[leaf][0]&1:return False
            leaves+=1;continue
        if index>=len(bs.nodes):return False
        plane,back,front=bs.nodes[index];n=bs.plane_of(plane);lo=hi=-n[3]
        for k in range(3):
            a=n[k]*bounds[k][0];b=n[k]*bounds[k][1];lo+=min(a,b);hi+=max(a,b)
        epsilon=1e-5*(1+abs(lo)+abs(hi))
        if lo<=epsilon:pending.append(back)
        if hi>=-epsilon:pending.append(front)
    return leaves>0

def rotated(bounds,yaw):
    c,s=math.cos(yaw),math.sin(yaw)
    return bbox([(c*x-s*y,s*x+c*y,z) for x,y,z in corners(bounds)])

def ground_candidate(bs,anchor,offset,local,yaw,vehicle=False):
    xy=[anchor[0]+offset[0],anchor[1]+offset[1]];shape=rotated(local,yaw)
    zs=[];surfaces=[]
    # Real downward polygon hits at all projected box footprint corners,
    # edge midpoints and center. This rejects ledges/steep pads conservatively.
    center=[(b[0]+b[1])/2 for b in shape];extent=[(b[1]-b[0])/2 for b in shape]
    for dx,dy in [(0,0),(-1,-1),(-1,1),(1,-1),(1,1),(-1,0),(1,0),(0,-1),(0,1)]:
        point=(xy[0]+center[0]+dx*extent[0],xy[1]+center[1]+dy*extent[1],anchor[2]+2.5)
        hit=bs.test_vector(point,(0,0,-6))
        if not hit:return None
        t,surface=hit;n=bs.plane_of(bs.surfaces[surface][0])
        if n[2]<0.85:return None
        zs.append(point[2]-6*t);surfaces.append(surface)
    variation=max(zs)-min(zs)
    if variation>(0.3 if vehicle else 0.18):return None
    origin=[xy[0],xy[1],max(zs)-shape[2][0]+0.06]
    bounds=[[shape[k][0]+origin[k],shape[k][1]+origin[k]] for k in range(3)]
    if not empty_aabb(bs,bounds):return None
    return {'position':origin,'yaw':yaw,'BSP_AABB':bounds,'ground_height_range':[min(zs),max(zs)],'ground_surface_indices':sorted(set(surfaces)),'full_BSP_box_empty':True}

def resolve_placements(m, donor, defs, plan, *, aircraft_donor=None):
    """Resolve authored anchor recipes before any scenario row is appended.

    Incompatible geometry fails the conversion; never silently omit an item or
    place a body through solid BSP. Source row positions remain read-only.
    """
    fields=lambda name:{f.name:o for o,f in defs.fields(name)}
    sf=fields('Scenario')
    def rows(field,kind):
        n,p=struct.unpack('<iI',m.read(m.scenario.address+sf[field],8));size=defs.size(kind)
        return [m.read(p+i*size,size) for i in range(n)]
    equipment=rows('netgame equipment','ScenarioNetgameEquipment')
    vehicles=rows('vehicles','ScenarioVehicle')
    tags={'Flamethrower':r'weapons\flamethrower\flamethrower',
          'Fuel Rod':r'weapons\plasma_cannon\plasma_cannon',
          'Rocket Hog':r'vehicles\rwarthog\rwarthog',
          'Banshee':r'vehicles\banshee\banshee_mp'}
    bsp_tag=m.bsps[0][0]
    _,pointer=struct.unpack('<iI',m.read(bsp_tag.address+fields('ScenarioStructureBSP')['collision bsp'],8))
    bsp=CollisionBSP.read(m,pointer)
    result={'weapons':[],'vehicles':[]}
    for category in result:
        for row in plan[category]:
            vehicle=category=='vehicles';kind=row.get('kind','Rocket Hog') if vehicle else row['kind']
            raw=(vehicles if vehicle else equipment)[row['anchor_row']]
            anchor=struct.unpack_from('<3f',raw,8 if vehicle else 64)
            yaw=0.
            if vehicle:
                flags={}
                for flag in rows('netgame flags','ScenarioNetgameFlags'):
                    typ,team=struct.unpack_from('<2h',flag,16)
                    if typ==0 and team in (0,1):flags[team]=struct.unpack_from('<3f',flag)
                if len(flags)!=2:raise ValueError('paired CTF anchors required for vehicle heading')
                center=[sum(flags[t][k] for t in (0,1))/2 for k in range(3)]
                yaw=math.atan2(center[1]-anchor[1],center[0]-anchor[0])
            source=aircraft_donor if kind=='Banshee' else donor
            if source is None:raise ValueError('Banshee placements require an explicit aircraft donor')
            tag=next(t for t in source.tags if t.name==tags[kind] and t.group_name in ('weap','vehi'))
            bounds=object_bounds(source,tag,defs)['bounds']
            placed=ground_candidate(bsp,anchor,row['offset_xy'],bounds,yaw,vehicle)
            if placed is None:raise ValueError(f'{m.name}: no clear supported {kind} placement at anchor {row["anchor_row"]}')
            # Verify the actual serialized coordinates/heading, not only the
            # double-precision intermediate used by the geometry calculation.
            f32=lambda x:struct.unpack('<f',struct.pack('<f',x))[0]
            position=[f32(v) for v in placed['position']];yaw=f32(yaw)
            shape=rotated(bounds,yaw)
            box=[[position[k]+shape[k][0],position[k]+shape[k][1]] for k in range(3)]
            if not empty_aabb(bsp,box):raise ValueError('rounded placement intersects solid BSP')
            supported=ground_candidate(bsp,(position[0],position[1],anchor[2]),(0,0),bounds,yaw,vehicle)
            if supported is None or position[2]+shape[2][0]-supported['ground_height_range'][1]<0.05:
                raise ValueError('rounded placement lacks sampled ground clearance')
            result[category].append({**row,'position':position,'yaw':yaw})
    return result

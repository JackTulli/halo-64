/*
VECTOR_TREE.C

symbols in this file:
000FFF50 00d0:
	_vector_tree_new (0000)
00100020 0020:
	_vector_tree_delete (0000)
00100040 0040:
	_code_00100040 (0000)
00100080 0050:
	_code_00100080 (0000)
001000D0 0160:
	_vector_tree_find (0000)
*/

#include "cseries/cseries.h"

struct dynamic_array
{
	long element_size;
	long count;
	void *elements;
};

struct vector_tree_node
{
	long value;
	long lower_child_index;
	long equal_child_index;
	long upper_child_index;
};

typedef void *(*vector_tree_get_vector)(void *context, long value);
typedef long (*vector_tree_compare_component)(void *context, const void *vector, const void *other_vector, short component_index);

struct vector_tree
{
	long root_index;
	struct dynamic_array nodes;
	short component_count;
	short pad;
	void *context;
	vector_tree_get_vector get_vector;
	vector_tree_compare_component compare_component;
};

void dynamic_array_new(struct dynamic_array *array, long element_size);
void dynamic_array_delete(struct dynamic_array *array);
long dynamic_array_add_element(struct dynamic_array *array);
void *dynamic_array_get_element(struct dynamic_array *array, long index, long element_size);

void vector_tree_new(
	struct vector_tree *tree,
	short component_count,
	void *context,
	vector_tree_get_vector get_vector,
	vector_tree_compare_component compare_component)
{
	match_assert("c:\\halo\\SOURCE\\math\\vector_tree.c", 43, tree);
	match_assert("c:\\halo\\SOURCE\\math\\vector_tree.c", 44, get_vector);
	match_assert("c:\\halo\\SOURCE\\math\\vector_tree.c", 45, compare_component);
	match_assert("c:\\halo\\SOURCE\\math\\vector_tree.c", 46, component_count>0);

	dynamic_array_new(&tree->nodes, sizeof(struct vector_tree_node));
	tree->component_count = component_count;
	tree->compare_component = compare_component;
	tree->root_index = NONE;
	tree->context = context;
	tree->get_vector = get_vector;
}

void vector_tree_delete(
	struct vector_tree *tree)
{
	dynamic_array_delete(&tree->nodes);
}

static boolean vectors_equal(
	struct vector_tree *tree,
	const void *vector,
	const void *other_vector,
	short component_index)
{
	while (component_index < tree->component_count)
	{
		if (tree->compare_component(tree->context, vector, other_vector, component_index))
			return FALSE;

		component_index++;
	}

	return TRUE;
}

static void vector_tree_add_node(
	struct vector_tree *tree,
	struct vector_tree_node **node_reference)
{
	long node_index = dynamic_array_add_element(&tree->nodes);
	if (node_index != NONE)
	{
		struct vector_tree_node *node = dynamic_array_get_element(&tree->nodes, node_index, sizeof(struct vector_tree_node));
		node->lower_child_index = NONE;
		node->equal_child_index = NONE;
		node->upper_child_index = NONE;
		*node_reference = node;
	}
	else
	{
		*node_reference = NULL;
	}
}

boolean vector_tree_find(
	struct vector_tree *tree,
	const void *vector,
	struct vector_tree_node **index_reference)
{
	long *node_index_reference = &tree->root_index;
	short component_index = 0;

	match_assert("c:\\halo\\SOURCE\\math\\vector_tree.c", 74, tree);
	match_assert("c:\\halo\\SOURCE\\math\\vector_tree.c", 75, vector);
	match_assert("c:\\halo\\SOURCE\\math\\vector_tree.c", 76, index_reference);

	while (*node_index_reference != NONE)
	{
		struct vector_tree_node *node = dynamic_array_get_element(&tree->nodes, *node_index_reference, sizeof(struct vector_tree_node));
		const void *other_vector = tree->get_vector(tree->context, node->value);
		long comparison = tree->compare_component(tree->context, vector, other_vector, component_index);

		if (comparison < 0)
		{
			node_index_reference = &node->lower_child_index;
		}
		else if (comparison > 0)
		{
			node_index_reference = &node->upper_child_index;
		}
		else
		{
			component_index++;
			if (vectors_equal(tree, vector, other_vector, component_index))
			{
				*index_reference = node;
				return TRUE;
			}

			node_index_reference = &node->equal_child_index;
		}
	}

	*node_index_reference = tree->nodes.count;
	vector_tree_add_node(tree, index_reference);
	return FALSE;
}

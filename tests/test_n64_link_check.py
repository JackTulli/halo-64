import importlib.util
from pathlib import Path
from types import SimpleNamespace
import unittest
from unittest.mock import patch

spec = importlib.util.spec_from_file_location('n64_link_check', Path(__file__).parents[1] / 'tools/n64_link_check.py')
checker = importlib.util.module_from_spec(spec)
spec.loader.exec_module(checker)


class LinkCheckerTests(unittest.TestCase):
    def test_long_absolute_paths_stay_below_budget(self):
        objects = [f'C:/work with spaces/{"deep/" * 30}{i}.o' for i in range(353)]
        batches = list(checker.object_batches(objects))
        self.assertGreater(len(batches), 1)
        self.assertEqual([obj for batch in batches for obj in batch], objects)
        self.assertTrue(all(len(checker.subprocess.list2cmdline([checker.NM, '-A', *batch])) <= 16000 for batch in batches))

    def test_cross_batch_definition_and_missing_data(self):
        outputs = ['first.o: w found_later\nfirst.o: v missing_data\nfirst.o: U ordinary\n',
                   'second.o:0000 T found_later\nsecond.o: w missing_func\n']
        with patch.object(checker, 'object_batches', return_value=iter([['first.o'], ['second.o']])), \
             patch.object(checker.subprocess, 'run', side_effect=[SimpleNamespace(stdout=x) for x in outputs]):
            self.assertEqual(checker.missing_references(['first.o', 'second.o']), ['missing_data', 'missing_func'])

    def test_empty_and_oversized_argument(self):
        with patch.object(checker.subprocess, 'run') as run:
            self.assertEqual(checker.missing_references([]), [])
            run.assert_not_called()
        with self.assertRaises(ValueError):
            list(checker.object_batches(['x' * 17000]))

    def test_nm_failure_is_not_accepted_as_clean(self):
        with patch.object(checker.subprocess, 'run', side_effect=checker.subprocess.CalledProcessError(1, 'nm')):
            with self.assertRaises(checker.subprocess.CalledProcessError):
                checker.missing_references(['broken.o'])


if __name__ == '__main__':
    unittest.main()

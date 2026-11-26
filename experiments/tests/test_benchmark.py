import csv
import os
import sys
import tempfile
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(HERE))

import run_benchmark as rb  # noqa: E402
import summarize as sm  # noqa: E402


def write_file(path, text):
    with open(path, "w", encoding="utf-8", newline="") as f:
        f.write(text)


class BenchmarkTest(unittest.TestCase):
    def setUp(self):
        self._td = tempfile.TemporaryDirectory()
        self.dir = self._td.name
        self.addCleanup(self._td.cleanup)
        os.makedirs(os.path.join(self.dir, "A"))
        self.vrp = os.path.join(self.dir, "A", "A-n32-k5.vrp")
        write_file(self.vrp, "NAME : x\nDIMENSION : 32\n")
        self.bks_path = os.path.join(self.dir, "bks.csv")
        write_file(self.bks_path,
                   "instance,set,customers,k,bks,optimal,source\n"
                   "A-n32-k5,A,31,5,784,true,x\n")

    def test_read_bks_and_missing(self):
        self.assertEqual(rb.read_bks(self.bks_path)["A-n32-k5"]["bks"], "784")
        self.assertEqual(rb.read_bks(os.path.join(self.dir, "none.csv")), {})

    def test_gap(self):
        self.assertEqual(rb.compute_gap(800, 784), round(16 / 784 * 100, 4))
        self.assertEqual(rb.compute_gap(800, None), "")

    def test_instance_info_with_and_without_bks(self):
        info = rb.instance_info(self.vrp, rb.read_bks(self.bks_path))
        self.assertEqual(info, ("A-n32-k5", "A", 31, 5, 784.0))
        info = rb.instance_info(self.vrp, {})
        self.assertEqual(info, ("A-n32-k5", "A", 31, 5, None))

    def test_row_vehicles_minus_k(self):
        info = rb.instance_info(self.vrp, rb.read_bks(self.bks_path))
        row = rb.make_row(info, "nn", {"status": "ok", "total_distance": 800,
                                       "num_routes": 7}, "abc")
        self.assertEqual(row["vehicles_minus_k"], 2)
        self.assertEqual(list(row.keys()), rb.COLUMNS)

    def test_resume(self):
        out = os.path.join(self.dir, "r.csv")
        calls = []
        results = [{"status": "error", "message": "x"}]

        def fake(path, args):
            calls.append(path)
            return results[0] if results else {
                "status": "ok", "total_distance": 790, "num_routes": 5,
                "construct_ms": 1, "total_ms": 2}

        cfg = [("nn", ["--method", "nn"])]
        bks = rb.read_bks(self.bks_path)
        self.assertEqual(rb.run_all([self.vrp], cfg, bks, out, 2, fake, "s"), (0, 1, 0))
        results.clear()  # lần sau thành công: khóa chỉ có lỗi nên chạy lại
        self.assertEqual(rb.run_all([self.vrp], cfg, bks, out, 2, fake, "s"), (1, 0, 0))
        self.assertEqual(rb.run_all([self.vrp], cfg, bks, out, 2, fake, "s"), (0, 0, 1))
        self.assertEqual(len(calls), 2)
        rows = rb.read_rows(out)
        self.assertEqual(len(rows), 2)
        eff = rb.effective_rows(rows)
        self.assertEqual(len(eff), 1)
        self.assertEqual(list(eff.values())[0]["status"], "ok")

    def test_summarize_and_warning(self):
        p = os.path.join(self.dir, "s.csv")
        with open(p, "w", newline="", encoding="utf-8") as f:
            w = csv.DictWriter(f, fieldnames=rb.COLUMNS)
            w.writeheader()
            base = dict.fromkeys(rb.COLUMNS, "")
            base.update(set="A", config="nn", status="ok", num_routes="6",
                        vehicles_minus_k="1", total_ms="2")
            w.writerow(dict(base, instance="i1", gap_pct="10"))
            w.writerow(dict(base, instance="i2", gap_pct="30"))
            w.writerow(dict(base, instance="i3", gap_pct="-1", vehicles_minus_k="0"))
            w.writerow(dict(base, instance="i1", gap_pct="20"))  # hiệu lực
        eff = sm.load_effective([p])
        self.assertEqual(len(eff), 3)
        row = sm.stats_row(list(eff.values()))
        self.assertEqual(row[:3], ["3", "3", "16.33"])
        text = sm.summarize(eff)
        self.assertIn("A/i3", text)
        self.assertNotIn("A/i1 [", text)


    def test_warning_only_when_m_equals_k(self):
        def row(name, gap, d):
            return {"set": "P", "instance": name, "config": "cw",
                    "gap_pct": gap, "vehicles_minus_k": d}
        eff = {1: row("same-k", "-0.5", "0"), 2: row("fewer", "-0.5", "-1"),
               3: row("more", "-0.5", "1"), 4: row("positive", "2", "0")}
        w = sm.warnings(eff)
        self.assertEqual(len(w), 1)
        self.assertIn("P/same-k", w[0])


if __name__ == "__main__":
    unittest.main()

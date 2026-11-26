#include "../src.hpp"
#include <cassert>
#include <limits>

int main() {
    char answer[] = "abc\ndef\n";
    BaseJudger *icpc = new ICPCJudger(100, 100, answer);
    answer[0] = 'x';
    assert(icpc->GetScore() == 0);
    icpc->Submit(101, 100, "abc\ndef\n");
    assert(icpc->GetScore() == 0);
    icpc->Submit(100, 100, "abc\ndef\n");
    assert(icpc->GetScore() == 100);
    icpc->Submit(0, 0, "wrong");
    assert(icpc->GetScore() == 100);
    delete icpc;

    OIJudger oi(100, 100, "abc  \ndef\n");
    oi.Submit(100, 100, "abc\ndef   \n");
    assert(oi.GetScore() == 100);
    oi.Submit(0, 0, "abc\ndef\n");
    assert(oi.GetScore() == 0);
    oi.Submit(0, 0, "abc\ndef\n");
    assert(oi.GetScore() == 0);
    for (const char *wrong : {" abc\ndef\n", "a bc\ndef\n", "abc\ndef", "abc\ndef\n\n", "abc\t\ndef\n"}) {
        OIJudger judger(100, 100, "abc\ndef\n");
        judger.Submit(0, 0, wrong);
        assert(judger.GetScore() == 0);
    }
    OIJudger final_spaces(1, 1, "abc   ");
    final_spaces.Submit(1, 1, "abc");
    assert(final_spaces.GetScore() == 100);

    for (size_t time = 0; time <= 12; ++time) {
        for (size_t memory = 0; memory <= 12; ++memory) {
            SpacialJudger judger(10, 10, 3, 3, "ok");
            const size_t ts = time <= 3 ? 100 : time >= 10 ? 0 : (10 - time) * 100 / 7;
            const size_t ms = memory <= 3 ? 100 : memory >= 10 ? 0 : (10 - memory) * 100 / 7;
            judger.Submit(time, memory, "wrong");
            assert(judger.GetScore() == 0);
            judger.Submit(time, memory, "ok");
            assert(judger.GetScore() == ts * ms / 100);
            judger.Submit(0, 0, "ok");
            judger.Submit(100, 100, "ok");
            assert(judger.GetScore() == 100);
        }
    }
    const size_t maximum = std::numeric_limits<size_t>::max();
    SpacialJudger large(maximum, maximum, 0, 0, "ok");
    large.Submit(maximum / 2, maximum / 2, "ok");
    assert(large.GetScore() == 25);
    SpacialJudger equal(10, 10, 10, 10, "ok");
    equal.Submit(10, 10, "ok");
    assert(equal.GetScore() == 100);
}

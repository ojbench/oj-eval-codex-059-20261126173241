#ifndef OJ_JUDGERS_HPP
#define OJ_JUDGERS_HPP

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>

class BaseJudger {
public:
    BaseJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : answer_(new char[strlen(answer) + 1]),
          time_limit_(time_limit), memory_limit_(memory_limit), score_(0) {
        strcpy(answer_, answer);
    }

    virtual void Submit(size_t time, size_t memory, const char *output) = 0;

    size_t GetScore() const { return score_; }

    virtual ~BaseJudger() {
        delete[] answer_;
    }

protected:
    char *answer_;
    const size_t time_limit_;
    const size_t memory_limit_;
    size_t score_;

    virtual bool CheckAnswer(const char *output) const {
        return strcmp(answer_, output) == 0;
    }
};

class ICPCJudger : public BaseJudger {
public:
    ICPCJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            score_ = 100;
        }
    }
};

class OIJudger : public BaseJudger {
public:
    OIJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer), submitted_(false) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        score_ = !submitted_ && time <= time_limit_ && memory <= memory_limit_
                         && CheckAnswer(output) ? 100 : 0;
        submitted_ = true;
    }

protected:
    bool CheckAnswer(const char *output) const override {
        const char *answer = answer_;
        while (true) {
            const char *answer_end = answer;
            const char *output_end = output;
            while (*answer_end && *answer_end != '\n') ++answer_end;
            while (*output_end && *output_end != '\n') ++output_end;

            const char *answer_trim = answer_end;
            const char *output_trim = output_end;
            while (answer_trim > answer && answer_trim[-1] == ' ') --answer_trim;
            while (output_trim > output && output_trim[-1] == ' ') --output_trim;
            const size_t length = answer_trim - answer;
            if (length != static_cast<size_t>(output_trim - output) ||
                strncmp(answer, output, length) != 0) return false;
            if (*answer_end != *output_end) return false;
            if (*answer_end == '\0') return true;
            answer = answer_end + 1;
            output = output_end + 1;
        }
    }

private:
    bool submitted_;
};

class SpacialJudger : public BaseJudger {
public:
    SpacialJudger(size_t time_limit, size_t memory_limit,
                  size_t full_score_time, size_t full_score_memory,
                  const char *answer)
        : BaseJudger(time_limit, memory_limit, answer),
          full_score_time_(full_score_time), full_score_memory_(full_score_memory) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        if (!CheckAnswer(output)) return;
        const size_t time_score = ComponentScore(time, full_score_time_, time_limit_);
        const size_t memory_score = ComponentScore(memory, full_score_memory_, memory_limit_);
        const size_t score = time_score * memory_score / 100;
        if (score > score_) score_ = score;
    }

private:
    const size_t full_score_time_;
    const size_t full_score_memory_;

    static size_t ComponentScore(size_t value, size_t full, size_t limit) {
        if (value <= full) return 100;
        if (value >= limit) return 0;
        // Widen before multiplication to keep integer flooring exact without overflow.
        return static_cast<unsigned __int128>(limit - value) * 100 / (limit - full);
    }
};

#endif

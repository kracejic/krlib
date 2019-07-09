#include <stdexcept>

namespace kr
{
// Do not forget this
// kr::CanaryObject::State kr::CanaryObject::states[256];
class CanaryObject
{
  private:
  public:
    void throwIfInvalid() const
    {
        if (state == State::undef)
            throw std::runtime_error("State::undef");
        if (state == State::movedFrom)
            throw std::runtime_error("State::movedFrom");
        if (state == State::destruct)
            throw std::runtime_error("State::destruct");
    }
    int _id = 0;
    int val = 0;
    CanaryObject()
    {
        state = State::construct1;
        states[_id] = State::construct1;
    }
    CanaryObject(int i)
    {
        state = State::construct2;
        states[_id] = State::construct2;
        val = i;
    }
    CanaryObject(int i, int canaryIndex)
    {
        _id = canaryIndex;
        state = State::construct2;
        states[_id] = State::construct2;
        val = i;
    }
    CanaryObject(const CanaryObject& p)
    {
        p.throwIfInvalid();
        _id = p._id;
        state = State::copyConst;
        states[_id] = State::copyConst;
    };
    CanaryObject& operator=(const CanaryObject& p)
    {
        p.throwIfInvalid();
        _id = p._id;
        state = State::copyAssign;
        states[_id] = State::copyAssign;
        return *this;
    };
    CanaryObject(CanaryObject&& p)
    {
        p.throwIfInvalid();
        _id = p._id;
        p._id = 255;
        val = p.val;
        p.val = 255;
        p.state = State::movedFrom;
        state = State::moveConst;
        states[_id] = State::moveConst;
    };
    CanaryObject& operator=(CanaryObject&& p)
    {
        p.throwIfInvalid();
        _id = p._id;
        p._id = 255;
        val = p.val;
        p.val = 255;
        p.state = State::movedFrom;
        state = State::moveAssign;
        states[_id] = State::moveAssign;
        return *this;
    };
    enum class State
    {
        undef,      // 0
        construct1, // 1
        construct2, // 2
        copyConst,  // 3
        copyAssign, // 4
        moveConst,  // 5
        moveAssign, // 6
        movedFrom,  // 7
        destruct,   // 8
        vdestruct   // 9
    };
    bool isValid()
    {
        switch (state)
        {
            default:
                return false;
            case State::construct1:
            case State::construct2:
            case State::copyConst:
            case State::copyAssign:
            case State::moveConst:
            case State::moveAssign:
                break;
        }
        if (_id < 0 || _id > 256)
            return false;
        return true;
    }
    virtual ~CanaryObject()
    {
        state = State::destruct;
        states[_id] = State::destruct;
    }

    State state;
    static State states[256];
    static void initStates()
    {
        for (int i = 0; i < 256; ++i)
            states[i] = State::undef;
    }
};


} /* kr */

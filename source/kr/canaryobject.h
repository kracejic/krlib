namespace kr
{
// Do not forget this
// kr::CanaryObject::State kr::CanaryObject::states[256];
class CanaryObject
{
  private:
  public:
    int id = 0;
    int val = 0;
    CanaryObject()
    {
        state = State::construct1;
        states[id] = State::construct1;
    }
    CanaryObject(int i)
    {
        state = State::construct2;
        states[id] = State::construct2;
        val = i;
    }
    CanaryObject(int i, int canaryIndex)
    {
        id = canaryIndex;
        state = State::construct2;
        states[id] = State::construct2;
        val = i;
    }
    CanaryObject(const CanaryObject& p)
    {
        id = p.id;
        state = State::copyConst;
        states[id] = State::copyConst;
    };
    CanaryObject& operator=(const CanaryObject& p)
    {
        id = p.id;
        state = State::copyAssign;
        states[id] = State::copyAssign;
        return *this;
    };
    CanaryObject(CanaryObject&& p)
    {
        id = p.id;
        p.id = 255;
        val = p.val;
        p.val = 255;
        p.state = State::movedFrom;
        state = State::moveConst;
        states[id] = State::moveConst;
    };
    CanaryObject& operator=(CanaryObject&& p)
    {
        id = p.id;
        p.id = 255;
        val = p.val;
        p.val = 255;
        p.state = State::movedFrom;
        state = State::moveAssign;
        states[id] = State::moveAssign;
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
        destruct    // 8
    };
    ~CanaryObject()
    {
        state = State::destruct;
        states[id] = State::destruct;
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

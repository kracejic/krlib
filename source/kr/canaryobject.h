namespace kr
{
// Do not forget this
// kr::CanaryObject::State kr::CanaryObject::states[256];
class CanaryObject
{
  private:
  public:
    int a = 0;
    int b = 0;
    CanaryObject()
    {
        state = State::construct1;
        states[a] = State::construct1;
    }
    CanaryObject(int i)
    {
        state = State::construct2;
        states[a] = State::construct2;
        b = i;
    }
    CanaryObject(int i, int canaryIndex)
    {
        a = canaryIndex;
        state = State::construct2;
        states[a] = State::construct2;
        b = i;
    }
    CanaryObject(const CanaryObject& p)
    {
        a = p.a;
        state = State::copyConst;
        states[a] = State::copyConst;
    };
    CanaryObject& operator=(const CanaryObject& p)
    {
        a = p.a;
        state = State::copyAssign;
        states[a] = State::copyAssign;
        return *this;
    };
    CanaryObject(CanaryObject&& p)
    {
        a = p.a;
        p.a = 255;
        p.state = State::movedFrom;
        state = State::moveConst;
        states[a] = State::moveConst;
    };
    CanaryObject& operator=(CanaryObject&& p)
    {
        a = p.a;
        p.a = 255;
        p.state = State::movedFrom;
        state = State::moveAssign;
        states[a] = State::moveAssign;
        return *this;
    };
    enum class State
    {
        undef,
        construct1,
        construct2,
        copyConst,
        copyAssign,
        moveConst,
        moveAssign,
        movedFrom,
        destruct
    };
    ~CanaryObject()
    {
        state = State::destruct;
        states[a] = State::destruct;
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

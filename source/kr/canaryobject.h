namespace kr
{
class CanaryObject
{
  private:
  public:
    int a;
    CanaryObject()
    {
        state = State::construct1;
    }
    CanaryObject(int i)
    {
        state = State::construct2;
        a = i;
    }
    CanaryObject(const CanaryObject& p)
    {
        a = p.a;
        state = State::copyConst;
    };
    CanaryObject& operator=(const CanaryObject& p)
    {
        a = p.a;
        state = State::copyAssign;
        return *this;
    };
    CanaryObject(CanaryObject&& p)
    {
        a = p.a;
        state = State::moveConst;
    };
    CanaryObject& operator=(CanaryObject&& p)
    {
        a = p.a;
        state = State::moveAssign;
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
        destruct
    };
    ~CanaryObject()
    {
        state = State::destruct;
    }

    static State state;
};

} /* kr */

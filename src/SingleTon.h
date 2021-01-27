class singleton {
protected:
  singleton() {}
private:
  static singleton* p;
public:
  static singleton* initance();
};

singleton* singleton::p = new singleton;

singleton* singleton::initance() {
  return p;
}


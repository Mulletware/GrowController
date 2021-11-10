
class DummySensor {
  public:
    DummySensor(int low = 60, int high = 61, int persistence = 25) {
      this->low = low;
      this->high = high;
      this->currentValue = high;
      this->isIncreasing = false;
      this->persistence = persistence;
      this->persist = persistence;
    };

    update() {
      if (persist == persistence) {
        if (this->isIncreasing) {
          this->currentValue++;
          if (this->currentValue == this->high) {
            this->isIncreasing = false;
          }
        } else {
          this->currentValue--;
          if (this->currentValue == this->low) {
            this->isIncreasing = true;
          }
        }

        this->persist = 0;
      } else {
        this->persist++;
      }
    }

    int getValue() {
      return this->currentValue;
    }

  private:
    bool isIncreasing;
    int low, high, currentValue, persist, persistence;
};

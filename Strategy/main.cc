// enum TaxBase {
//   CN_Tax,
//   US_Tax,
//   DE_Tax,
// };

// class SalesOrder {
//   TaxBase tax;
// public:
//   double CalculateTax() {
//     if(tax == CN_Tax) {/**/}
//     else if(tax == US_Tax) {/**/}
//     else if(tax == DE_Tax) {/**/}
//   }
// };

class Contex;
class TaxStrategy {
public:
  virtual double Calculate(const Contex& contex) = 0;
  virtual ~TaxStrategy(){};
};

class CNTax : public TaxStrategy {
public:
  double Calculate(const Contex& contex) override;
};

class USTax : public TaxStrategy {
public:
  double Calculate(const Contex& contex) override;
};

class SalesOrder {
private:
  TaxStrategy& strategy;
public:
  SalesOrder(TaxStrategy& tax_strategy) : strategy(tax_strategy){}
  double CalculateTax(const Contex& contex) {
    strategy.Calculate(contex);
  }
};
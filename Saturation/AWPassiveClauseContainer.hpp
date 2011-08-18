/**
 * @file AWPassiveClauseContainer.hpp
 * Defines the class AWPassiveClauseContainer
 * @since 31/12/2007 Manchester
 */

#ifndef __AWPassiveClauseContainer__
#define __AWPassiveClauseContainer__

#include "Lib/Comparison.hpp"
#include "Kernel/Clause.hpp"
#include "Kernel/ClauseQueue.hpp"
#include "ClauseContainer.hpp"


namespace Saturation {

using namespace Kernel;

class AgeQueue
: public ClauseQueue
{
public:
  AgeQueue(const Options& opt) : _opt(opt) {}
protected:

  virtual bool lessThan(Clause*,Clause*);

  friend class AWPassiveClauseContainer;

private:
  const Options& _opt;
};

class WeightQueue
  : public ClauseQueue
{
public:
  WeightQueue(const Options& opt) : _opt(opt) {}
protected:
  virtual bool lessThan(Clause*,Clause*);

  friend class AWPassiveClauseContainer;
private:
  const Options& _opt;
};

/**
 * Defines the class Passive of passive clauses
 * @since 31/12/2007 Manchester
 */
class AWPassiveClauseContainer
: public PassiveClauseContainer
{
public:
  AWPassiveClauseContainer(const Options& opt);
  virtual ~AWPassiveClauseContainer();
  void add(Clause* cl);

  void remove(Clause* cl);

  Clause* popSelected();
  /** True if there are no passive clauses */
  bool isEmpty() const
  { return _ageQueue.isEmpty() && _weightQueue.isEmpty(); }

  ClauseIterator iterator();

  void updateLimits(long long estReachableCnt);

  unsigned size() const { return _size; }

  void beforePassiveClauseWeightChange(Clause* cl);
  void afterPassiveClauseWeightChange(Clause* cl);


  static Comparison compareWeight(Clause* cl1, Clause* cl2, const Options& opt);
protected:
  void onLimitsUpdated(LimitsChangeType change);

private:

  void beforeClausePropChange(Clause* cl);
  void afterClausePropChange(Clause* cl);

  SubscriptionData _beforeClausePropUpdateSD;
  SubscriptionData _afterClausePropUpdateSD;

  /**
   * If between calls to beforePassiveClauseWeightChange() and
   * afterPassiveClauseWeightChange(), contains the clause that is being
   * updated. Otherwise contains zero.
   */
  Clause* _clauseHavingWeightChanged;
  /**
   * Value of this variable is being set in the afterPassiveClauseWeightChange() function.
   * True if the updated clause was actually inthe container. This will mean
   * that the clause will be inserted afterward.
   */
  bool _clauseHavingWeightChangedWasInContainer;

  /** The age queue, empty if _ageRatio=0 */
  AgeQueue _ageQueue;
  /** The weight queue, empty if _weightRatio=0 */
  WeightQueue _weightQueue;
  /** the age ratio */
  int _ageRatio;
  /** the weight ratio */
  int _weightRatio;
  /** current balance. If &lt;0 then selection by age, if &gt;0
   * then by weight */
  int _balance;

  unsigned _size;

  const Options& _opt;
}; // class AWPassiveClauseContainer

/**
 * Light-weight version of the AWPassiveClauseContainer that
 * is not linked to the SaturationAlgorithm
 */
class AWClauseContainer: public ClauseContainer
{
public:
  AWClauseContainer(const Options& opt);

  void add(Clause* cl);
  bool remove(Clause* cl);

  /**
   * Set age-weight ratio
   * @since 08/01/2008 flight Murcia-Manchester
   */
  void setAgeWeightRatio(int age,int weight)
  {
    ASS(age >= 0);
    ASS(weight >= 0);
    ASS(age > 0 || weight > 0);

    _ageRatio = age;
    _weightRatio = weight;
  }

  Clause* popSelected();
  /** True if there are no passive clauses */
  bool isEmpty() const;

  unsigned size() const { return _size; }

  static Comparison compareWeight(Clause* cl1, Clause* cl2);

private:
  /** The age queue, empty if _ageRatio=0 */
  AgeQueue _ageQueue;
  /** The weight queue, empty if _weightRatio=0 */
  WeightQueue _weightQueue;
  /** the age ratio */
  int _ageRatio;
  /** the weight ratio */
  int _weightRatio;
  /** current balance. If &lt;0 then selection by age, if &gt;0
   * then by weight */
  int _balance;

  unsigned _size;
};


};

#endif /* __AWPassiveClauseContainer__ */

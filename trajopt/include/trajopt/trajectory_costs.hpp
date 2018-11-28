#pragma once

/**
Simple quadratic costs on trajectory
*/

#include <trajopt/common.hpp>
#include <trajopt_sco/modeling.hpp>
#include <trajopt_utils/macros.h>

namespace trajopt
{
class TRAJOPT_API JointPosCost : public sco::Cost
{
public:
  JointPosCost(const sco::VarVector& vars, const Eigen::VectorXd& vals, const Eigen::VectorXd& coeffs);
  virtual sco::ConvexObjectivePtr convex(const DblVec& x, sco::Model* model);
  /** @brief Evaluate cost given the vector of values */
  virtual double value(const DblVec&);

private:
  /** @brief The variables being optimized. Used to properly index the vector being optimized */
  sco::VarVector vars_;
  /** @brief The target values. Cost is applied to difference between current value and this one */
  Eigen::VectorXd vals_;
  /** @brief The coefficients used to weight the cost */
  Eigen::VectorXd coeffs_;
  /** @brief Stores the cost as an expression */
  sco::QuadExpr expr_;
};

class TRAJOPT_API JointVelEqCost : public sco::Cost
{
public:
  /** @brief Forms error in QuadExpr - independent of penalty type */
  JointVelEqCost(const VarArray& traj,
                 const Eigen::VectorXd& coeffs,
                 const Eigen::VectorXd& targs,
                 int& first_step,
                 int& last_step);
  /** @brief Convexifies cost expression - In this case, it is already quadratic so there's nothing to do */
  virtual sco::ConvexObjectivePtr convex(const DblVec& x, sco::Model* model);
  /** @brief Numerically evaluate cost given the vector of values */
  virtual double value(const DblVec&);

private:
  /** @brief The variables being optimized. Used to properly index the vector being optimized */
  VarArray vars_;
  /** @brief The coefficients used to weight the cost */
  Eigen::VectorXd coeffs_;
  /** @brief Stores the cost as an expression */
  sco::QuadExpr expr_;
  /** @brief Vector of velocity targets */
  Eigen::VectorXd targs_;
  /** @brief First time step to which the term is applied */
  int first_step_;
  /** @brief Last time step to which the term is applied */
  int last_step_;

  // TODO: Add getVars
};

/**
 * @brief The JointVelIneqCost class
 * Assumes that the target is ...
 */
class TRAJOPT_API JointVelIneqCost : public sco::Cost
{
public:
  /** @brief Forms error in QuadExpr - independent of penalty type */
  JointVelIneqCost(const VarArray& traj,
                   const Eigen::VectorXd& coeffs,
                   const Eigen::VectorXd& targs,
                   const Eigen::VectorXd& upper_limits,
                   const Eigen::VectorXd& lower_limits,
                   int& first_step,
                   int& last_step);
  /** @brief Convexifies cost expression - In this case, it is already quadratic so there's nothing to do */
  virtual sco::ConvexObjectivePtr convex(const DblVec& x, sco::Model* model);
  /** @brief Numerically evaluate cost given the vector of values */
  virtual double value(const DblVec&);

private:
  /** @brief The variables being optimized. Used to properly index the vector being optimized */
  VarArray vars_;
  /** @brief The coefficients used to weight the cost */
  Eigen::VectorXd coeffs_;
  /** @brief Vector of upper tolerances */
  Eigen::VectorXd upper_tols_;
  /** @brief Vector of lower tolerances */
  Eigen::VectorXd lower_tols_;
  /** @brief Vector of velocity targets */
  Eigen::VectorXd targs_;
  /** @brief First time step to which the term is applied */
  int first_step_;
  /** @brief Last time step to which the term is applied */
  int last_step_;
  /** @brief Stores the costs as an expression. Will be length num_jnts*num_timesteps*2 */
  std::vector<sco::AffExpr> expr_vec_;

  // TODO: Add getVars
};

class TRAJOPT_API JointVelEqConstraint : public sco::EqConstraint
{
public:
  /** @brief Forms error in QuadExpr - independent of penalty type */
  JointVelEqConstraint(const VarArray& traj,
                       const Eigen::VectorXd& coeffs,
                       const Eigen::VectorXd& targs,
                       int& first_step,
                       int& last_step);
  /** @brief Convexifies cost expression - In this case, it is already quadratic so there's nothing to do */
  virtual sco::ConvexConstraintsPtr convex(const DblVec& x, sco::Model* model);
  /** @brief Numerically evaluate cost given the vector of values */
  virtual DblVec value(const DblVec&);
  /** Calculate constraint violations (positive part for inequality constraint,
   * absolute value for inequality constraint)*/
  DblVec violations(const DblVec& x);
  /** Sum of violations */
  double violation(const DblVec& x);

  //  TODO: VarVector getVars() { return VarVector(); }
private:
  /** @brief The variables being optimized. Used to properly index the vector being optimized */
  VarArray vars_;
  /** @brief The coefficients used to weight the cost */
  Eigen::VectorXd coeffs_;
  /** @brief Stores the costs as an expression. Will be length num_jnts*num_timesteps */
  std::vector<sco::AffExpr> expr_vec_;
  /** @brief Vector of velocity targets */
  Eigen::VectorXd targs_;
  /** @brief First time step to which the term is applied */
  int first_step_;
  /** @brief Last time step to which the term is applied */
  int last_step_;
};

class TRAJOPT_API JointVelIneqConstraint : public sco::IneqConstraint
{
public:
  /** @brief Forms error in QuadExpr - independent of penalty type */
  JointVelIneqConstraint(const VarArray& traj,
                         const Eigen::VectorXd& coeffs,
                         const Eigen::VectorXd& targs,
                         const Eigen::VectorXd& upper_limits,
                         const Eigen::VectorXd& lower_limits,
                         int& first_step,
                         int& last_step);
  /** @brief Convexifies cost expression - In this case, it is already quadratic so there's nothing to do */
  virtual sco::ConvexConstraintsPtr convex(const DblVec& x, sco::Model* model);
  /** @brief Numerically evaluate cost given the vector of values */
  virtual DblVec value(const DblVec&);

private:
  /** @brief The variables being optimized. Used to properly index the vector being optimized */
  VarArray vars_;
  /** @brief The coefficients used to weight the cost */
  Eigen::VectorXd coeffs_;
  /** @brief Vector of upper tolerances */
  Eigen::VectorXd upper_tols_;
  /** @brief Vector of lower tolerances */
  Eigen::VectorXd lower_tols_;
  /** @brief Vector of velocity targets */
  Eigen::VectorXd targs_;
  /** @brief First time step to which the term is applied */
  int first_step_;
  /** @brief Last time step to which the term is applied */
  int last_step_;
  /** @brief Stores the costs as an expression. Will be length num_jnts*num_timesteps*2 */
  std::vector<sco::AffExpr> expr_vec_;

  // TODO: Add getVars
};

class TRAJOPT_API JointAccEqCost : public sco::Cost
{
public:
  /** @brief Forms error in QuadExpr - independent of penalty type */
  JointAccEqCost(const VarArray& traj,
                 const Eigen::VectorXd& coeffs,
                 const Eigen::VectorXd& targs,
                 int& first_step,
                 int& last_step);
  /** @brief Convexifies cost expression - In this case, it is already quadratic so there's nothing to do */
  virtual sco::ConvexObjectivePtr convex(const DblVec& x, sco::Model* model);
  /** @brief Numerically evaluate cost given the vector of values */
  virtual double value(const DblVec&);

private:
  /** @brief The variables being optimized. Used to properly index the vector being optimized */
  VarArray vars_;
  /** @brief The coefficients used to weight the cost */
  Eigen::VectorXd coeffs_;
  /** @brief Stores the cost as an expression */
  sco::QuadExpr expr_;
  /** @brief Vector of velocity targets */
  Eigen::VectorXd targs_;
  /** @brief First time step to which the term is applied */
  int first_step_;
  /** @brief Last time step to which the term is applied */
  int last_step_;

  // TODO: Add getVars
};

class TRAJOPT_API JointAccIneqCost : public sco::Cost
{
public:
  /** @brief Forms error in a vector of AffExpr - independent of penalty type */
  JointAccIneqCost(const VarArray& traj,
                   const Eigen::VectorXd& coeffs,
                   const Eigen::VectorXd& targs,
                   const Eigen::VectorXd& upper_limits,
                   const Eigen::VectorXd& lower_limits,
                   int& first_step,
                   int& last_step);
  /** @brief Convexifies cost expression - In this case, it is already quadratic so there's nothing to do */
  virtual sco::ConvexObjectivePtr convex(const DblVec& x, sco::Model* model);
  /** @brief Numerically evaluate cost given the vector of values */
  virtual double value(const DblVec&);

private:
  /** @brief The variables being optimized. Used to properly index the vector being optimized */
  VarArray vars_;
  /** @brief The coefficients used to weight the cost */
  Eigen::VectorXd coeffs_;
  /** @brief Vector of upper tolerances */
  Eigen::VectorXd upper_tols_;
  /** @brief Vector of lower tolerances */
  Eigen::VectorXd lower_tols_;
  /** @brief Vector of velocity targets */
  Eigen::VectorXd targs_;
  /** @brief First time step to which the term is applied */
  int first_step_;
  /** @brief Last time step to which the term is applied */
  int last_step_;
  /** @brief Stores the costs as an expression. Will be length num_jnts*num_timesteps*2 */
  std::vector<sco::AffExpr> expr_vec_;

  // TODO: Add getVars
};

class TRAJOPT_API JointAccEqConstraint : public sco::EqConstraint
{
public:
  /** @brief Forms error in a vector of AffExpr - independent of penalty type */
  JointAccEqConstraint(const VarArray& traj,
                       const Eigen::VectorXd& coeffs,
                       const Eigen::VectorXd& targs,
                       int& first_step,
                       int& last_step);
  /** @brief Convexifies cost expression - In this case, it is already quadratic so there's nothing to do */
  virtual sco::ConvexConstraintsPtr convex(const DblVec& x, sco::Model* model);
  /** @brief Numerically evaluate cost given the vector of values */
  virtual DblVec value(const DblVec&);
  /** Calculate constraint violations (positive part for inequality constraint,
   * absolute value for inequality constraint)*/
  DblVec violations(const DblVec& x);
  /** Sum of violations */
  double violation(const DblVec& x);

  //  TODO: VarVector getVars() { return VarVector(); }
private:
  /** @brief The variables being optimized. Used to properly index the vector being optimized */
  VarArray vars_;
  /** @brief The coefficients used to weight the cost */
  Eigen::VectorXd coeffs_;
  /** @brief Stores the costs as an expression. Will be length num_jnts*num_timesteps */
  std::vector<sco::AffExpr> expr_vec_;
  /** @brief Vector of velocity targets */
  Eigen::VectorXd targs_;
  /** @brief First time step to which the term is applied */
  int first_step_;
  /** @brief Last time step to which the term is applied */
  int last_step_;
};

class TRAJOPT_API JointAccIneqConstraint : public sco::IneqConstraint
{
public:
  /** @brief Forms error in a vector of AffExpr - independent of penalty type */
  JointAccIneqConstraint(const VarArray& traj,
                         const Eigen::VectorXd& coeffs,
                         const Eigen::VectorXd& targs,
                         const Eigen::VectorXd& upper_limits,
                         const Eigen::VectorXd& lower_limits,
                         int& first_step,
                         int& last_step);
  /** @brief Convexifies cost expression - In this case, it is already quadratic so there's nothing to do */
  virtual sco::ConvexConstraintsPtr convex(const DblVec& x, sco::Model* model);
  /** @brief Numerically evaluate cost given the vector of values */
  virtual DblVec value(const DblVec&);

private:
  /** @brief The variables being optimized. Used to properly index the vector being optimized */
  VarArray vars_;
  /** @brief The coefficients used to weight the cost */
  Eigen::VectorXd coeffs_;
  /** @brief Vector of upper tolerances */
  Eigen::VectorXd upper_tols_;
  /** @brief Vector of lower tolerances */
  Eigen::VectorXd lower_tols_;
  /** @brief Vector of velocity targets */
  Eigen::VectorXd targs_;
  /** @brief First time step to which the term is applied */
  int first_step_;
  /** @brief Last time step to which the term is applied */
  int last_step_;
  /** @brief Stores the costs as an expression. Will be length num_jnts*num_timesteps*2 */
  std::vector<sco::AffExpr> expr_vec_;

  // TODO: Add getVars
};
/**
 * @brief The JointJerkEqCost class
 *
 *     Calculated with central finite difference. TODO: calc first/last 2 pnts using fwd/backward
 *      https://en.wikipedia.org/wiki/Finite_difference_coefficient
 */
class TRAJOPT_API JointJerkEqCost : public sco::Cost
{
public:
  /** @brief Forms error in QuadExpr - independent of penalty type */
  JointJerkEqCost(const VarArray& traj,
                  const Eigen::VectorXd& coeffs,
                  const Eigen::VectorXd& targs,
                  int& first_step,
                  int& last_step);
  /** @brief Convexifies cost expression - In this case, it is already quadratic so there's nothing to do */
  virtual sco::ConvexObjectivePtr convex(const DblVec& x, sco::Model* model);
  /** @brief Numerically evaluate cost given the vector of values */
  virtual double value(const DblVec&);

private:
  /** @brief The variables being optimized. Used to properly index the vector being optimized */
  VarArray vars_;
  /** @brief The coefficients used to weight the cost */
  Eigen::VectorXd coeffs_;
  /** @brief Stores the cost as an expression */
  sco::QuadExpr expr_;
  /** @brief Vector of velocity targets */
  Eigen::VectorXd targs_;
  /** @brief First time step to which the term is applied */
  int first_step_;
  /** @brief Last time step to which the term is applied */
  int last_step_;

  // TODO: Add getVars
};

class TRAJOPT_API JointJerkIneqCost : public sco::Cost
{
public:
  /** @brief Forms error in a vector of AffExpr - independent of penalty type */
  JointJerkIneqCost(const VarArray& traj,
                    const Eigen::VectorXd& coeffs,
                    const Eigen::VectorXd& targs,
                    const Eigen::VectorXd& upper_limits,
                    const Eigen::VectorXd& lower_limits,
                    int& first_step,
                    int& last_step);
  /** @brief Convexifies cost expression - In this case, it is already quadratic so there's nothing to do */
  virtual sco::ConvexObjectivePtr convex(const DblVec& x, sco::Model* model);
  /** @brief Numerically evaluate cost given the vector of values */
  virtual double value(const DblVec&);

private:
  /** @brief The variables being optimized. Used to properly index the vector being optimized */
  VarArray vars_;
  /** @brief The coefficients used to weight the cost */
  Eigen::VectorXd coeffs_;
  /** @brief Vector of upper tolerances */
  Eigen::VectorXd upper_tols_;
  /** @brief Vector of lower tolerances */
  Eigen::VectorXd lower_tols_;
  /** @brief Vector of velocity targets */
  Eigen::VectorXd targs_;
  /** @brief First time step to which the term is applied */
  int first_step_;
  /** @brief Last time step to which the term is applied */
  int last_step_;
  /** @brief Stores the costs as an expression. Will be length num_jnts*num_timesteps*2 */
  std::vector<sco::AffExpr> expr_vec_;

  // TODO: Add getVars
};

class TRAJOPT_API JointJerkEqConstraint : public sco::EqConstraint
{
public:
  /** @brief Forms error in a vector of AffExpr - independent of penalty type */
  JointJerkEqConstraint(const VarArray& traj,
                        const Eigen::VectorXd& coeffs,
                        const Eigen::VectorXd& targs,
                        int& first_step,
                        int& last_step);
  /** @brief Convexifies cost expression - In this case, it is already quadratic so there's nothing to do */
  virtual sco::ConvexConstraintsPtr convex(const DblVec& x, sco::Model* model);
  /** @brief Numerically evaluate cost given the vector of values */
  virtual DblVec value(const DblVec&);
  /** Calculate constraint violations (positive part for inequality constraint,
   * absolute value for inequality constraint)*/
  DblVec violations(const DblVec& x);
  /** Sum of violations */
  double violation(const DblVec& x);

  //  TODO: VarVector getVars() { return VarVector(); }
private:
  /** @brief The variables being optimized. Used to properly index the vector being optimized */
  VarArray vars_;
  /** @brief The coefficients used to weight the cost */
  Eigen::VectorXd coeffs_;
  /** @brief Stores the costs as an expression. Will be length num_jnts*num_timesteps */
  std::vector<sco::AffExpr> expr_vec_;
  /** @brief Vector of velocity targets */
  Eigen::VectorXd targs_;
  /** @brief First time step to which the term is applied */
  int first_step_;
  /** @brief Last time step to which the term is applied */
  int last_step_;
};

class TRAJOPT_API JointJerkIneqConstraint : public sco::IneqConstraint
{
public:
  /** @brief Forms error in a vector of AffExpr - independent of penalty type */
  JointJerkIneqConstraint(const VarArray& traj,
                          const Eigen::VectorXd& coeffs,
                          const Eigen::VectorXd& targs,
                          const Eigen::VectorXd& upper_limits,
                          const Eigen::VectorXd& lower_limits,
                          int& first_step,
                          int& last_step);
  /** @brief Convexifies cost expression - In this case, it is already quadratic so there's nothing to do */
  virtual sco::ConvexConstraintsPtr convex(const DblVec& x, sco::Model* model);
  /** @brief Numerically evaluate cost given the vector of values */
  virtual DblVec value(const DblVec&);

private:
  /** @brief The variables being optimized. Used to properly index the vector being optimized */
  VarArray vars_;
  /** @brief The coefficients used to weight the cost */
  Eigen::VectorXd coeffs_;
  /** @brief Vector of upper tolerances */
  Eigen::VectorXd upper_tols_;
  /** @brief Vector of lower tolerances */
  Eigen::VectorXd lower_tols_;
  /** @brief Vector of velocity targets */
  Eigen::VectorXd targs_;
  /** @brief First time step to which the term is applied */
  int first_step_;
  /** @brief Last time step to which the term is applied */
  int last_step_;
  /** @brief Stores the costs as an expression. Will be length num_jnts*num_timesteps*2 */
  std::vector<sco::AffExpr> expr_vec_;

  // TODO: Add getVars
};
}  // namespace trajopt

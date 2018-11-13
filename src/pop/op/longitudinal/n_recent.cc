/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#include "n_recent.h"

namespace FT{
    
    namespace Pop{
        namespace Op{
            NodeRecent::NodeRecent()
            {
                name = "recent";
	            otype = 'f';
	            arity['z'] = 1;
	            complexity = 1;
            }

            /// Evaluates the node and updates the state states. 
            void NodeRecent::evaluate(const Data& data, State& state)
            {
                ArrayXd tmp(state.z.top().first.size());
                int x;
                
                for(x = 0; x < state.z.top().first.size(); x++)
                {
                    // find max time
                    ArrayXd::Index maxIdx; 
                    double maxtime = state.z.top().second[x].maxCoeff(&maxIdx);
                    // return value at max time 
                    tmp(x) = state.z.top().first[x](maxIdx);
                }

                state.z.pop();
                
                state.push<double>(tmp);
                
            }

            /// Evaluates the node symbolically
            void NodeRecent::eval_eqn(State& state)
            {
                state.push<double>("recent(" + state.zs.pop() + ")");
            }
            
            NodeRecent* NodeRecent::clone_impl() const { return new NodeRecent(*this); }

            NodeRecent* NodeRecent::rnd_clone_impl() const { return new NodeRecent(); }
        }
    }
}

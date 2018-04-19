/* FEWTWO
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef NODE_CONSTANT
#define NODE_CONSTANT

#include "node.h"

namespace FT{
	class NodeConstant : public Node
    {
    	public:
    		
    		float d_value;           ///< value, for k and x types
    		bool b_value;
    		
    		NodeConstant()
    		{
    			std::cerr << "error in nodeconstant.h : invalid constructor called";
				throw;
    		}

            /// declares a boolean constant
    		NodeConstant(bool& v)
    		{
    			name = "k_b";
    			otype = 'b';
    			arity['f'] = 0;
    			arity['b'] = 0;
    			complexity = 1;
    			b_value = v;
    		}

            /// declares a float constant
    		NodeConstant(const float& v)
    		{
    			name = "k_d";
    			otype = 'f';
    			arity['f'] = 0;
    			arity['b'] = 0;
    			complexity = 1;
    			d_value = v;
    		}
    		
            /// Evaluates the node and updates the stack states. 
            void evaluate(const MatrixXd& X, const VectorXd& y,
                          const std::map<string, std::pair<vector<ArrayXd>, vector<ArrayXd> > > &Z, 
			              Stacks& stack);

            /// Evaluates the node symbolically
            void eval_eqn(Stacks& stack)
            {
        		if (otype == 'b')
                    stack.bs.push(std::to_string(b_value));
                else 	
                    stack.fs.push(std::to_string(d_value));
            }
    		
        protected:
            NodeConstant* clone_impl() const override { return new NodeConstant(*this); };  
    };
    // Definition
#ifndef USE_CUDA    
    void NodeConstant::evaluate(const MatrixXd& X, const VectorXd& y,
                          const std::map<string, std::pair<vector<ArrayXd>, vector<ArrayXd> > > &Z, 
			              Stacks& stack)
    {
        if (otype == 'b')
            stack.b.push(ArrayXb::Constant(X.cols(),int(b_value)));
        else 	
            stack.f.push(ArrayXf::Constant(X.cols(),d_value));
    }
#else
    void NodeConstant::evaluate(const MatrixXd& X, const VectorXd& y,
                          const std::map<string, std::pair<vector<ArrayXd>, vector<ArrayXd> > > &Z, 
			              Stacks& stack)
    {
        if (otype == 'b')
        {
            GPU_Constant(stack.dev_b, b_value, stack.idx['b'], stack.N);
        }
        else
        {
            GPU_Constant(stack.dev_f, d_value, stack.idx['f'], stack.N);
        }

    }
#endif
}	

#endif
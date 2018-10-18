/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef NODE_OR
#define NODE_OR

#include "../node.h"

namespace FT{


    namespace Pop{
        namespace NodeSpace{
        	class NodeOr : public Node
            {
            	public:
            	
            		NodeOr();
            		
                    /// Evaluates the node and updates the stack states. 
                    void evaluate(const Data& data, Stacks& stack);
                    
                    /// Evaluates the node symbolically
                    void eval_eqn(Stacks& stack);
                    
                protected:
                    NodeOr* clone_impl() const override;

                    NodeOr* rnd_clone_impl() const override;
            };
        }
    }    
}	

#endif

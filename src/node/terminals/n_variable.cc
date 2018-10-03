/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/

#include "n_variable.h"
			
namespace FT{
    template <class T>
    NodeVariable<T>::NodeVariable(const size_t& l, char ntype, std::string n)
    {
        if (n.empty())
	        name = "x_" + std::to_string(l);
        else
            name = n;
	    otype = ntype;
	    complexity = 1;
	    loc = l;
    }

#ifndef USE_CUDA
    /// Evaluates the node and updates the stack states. 
    template <class T>		
    void NodeVariable<T>::evaluate(const Data& data, Stacks& stack)
    {
        stack.push<T>(data.X.row(loc).template cast<T>());
    }
#else
    template <class T>
    void NodeVariable<T>::evaluate(const Data& data, Stacks& stack)
    {
        if(otype == 'b')
        {
            ArrayXb tmp = data.X.row(loc).cast<bool>();
            GPU_Variable(stack.dev_b, tmp.data(), stack.idx[otype], stack.N);
        }
        else if (otype == 'c')
        {
            ArrayXi tmp = data.X.row(loc).cast<int>();
            GPU_Variable(stack.dev_c, tmp.data(), stack.idx[otype], stack.N);
        }
        else
        {
            ArrayXf tmp = data.X.row(loc).cast<float>() ;
            // std::cout << "NodeVariable:\n stack.dev_f: " << stack.dev_f
            //           << "\ntmp.data(): " << tmp.data() 
            //           << "\ntmp.size(): " << tmp.size()
            //           << "\nstack.idx[otype]: " << stack.idx[otype]
            //           << "\nstack.N: " << stack.N <<"\n";
            GPU_Variable(stack.dev_f, tmp.data(), stack.idx[otype], stack.N);
        }
    }
#endif

    /// Evaluates the node symbolically
    template <class T>
    void NodeVariable<T>::eval_eqn(Stacks& stack)
    {
        stack.push<T>(name);
    }

    template <class T>
    NodeVariable<T>* NodeVariable<T>::clone_impl() const { return new NodeVariable<T>(*this); }
      
    // rnd_clone is just clone_impl() for variable, since rand vars not supported
    template <class T>
    NodeVariable<T>* NodeVariable<T>::rnd_clone_impl() const { return new NodeVariable<T>(*this); }
    
    template class NodeVariable<bool>;
    template class NodeVariable<int>;
    template class NodeVariable<double>;
}
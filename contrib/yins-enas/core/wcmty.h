#ifndef core_wcmty
#define core_wcmty

// Headers (?)

// Community class to be inherited
template <class TEdgeW>
class Community {
public:
  
protected:
  Quality Q;
private:
  
};

// Modularity objective community  
template <class TEdgeW>
class ModularityCommunity : public Objective {
public:
  // Modularity quality
  class Quality {
    public:
      
    private:
      
  };
protected:
  
private:
  
};

namespace TSnap {

template <class Community, class TEdgeW, template <class> class TGraph>
void LouvainMethod(PGraph& Graph, TIntIntVH& CmtyVH, double& eps, int& min_moves, int& max_iters) {
  
  // Variables needed
  
  Community Cmty; // or Quality (?)
  double phase_improv = 0.0;
  bool phase_init = true;
  // ...
  
  while (phase_init || phase_improv > eps) { // quality improving
    
    phase_init = false; // TODO: deprecate this with a single initial phase 1
    
    // Phase 1 : MoveNode<Objective<Community> >(Graph, Q, NId)
    // -------
    
    // Variables needed
    
    int moves = 0, iters = 0;
    double iter_improv = 0.0, move_improv = 0.0;
    bool iter_init = true;
    
    while (iter_init || (iter_improv > eps && moves > min_moves && iters < max_iters)) { // quality improving and parameters not exceeded
      
      iter_init = false; // TODO: deprecate this with an initial phase 1
      
      // randomly permute nodes (?)
      
      for () { // nodes, randomly permuted
        
        if (Cmty.move_node_diff(NId, DstCmty) > best_improv) { // check if node move improves quality
          
          // update node move
          
        }
        
      }
      
      Cmty.move_node(NId, DstCmty); // execute node move with the best improvement in quality
      
    }
    
    // -------
    
    // break if not improved
    
    if (!(iter_improv > 0.0)) break; // TODO: deprecate this with an initial phase 1
    
    // Phase 2 : CollapseGraph(Graph, CmtyGraph, CmtyV);
    // -------
    
    for (i) { // for node in graph
      
      if (!IsNode(Cmty.GetDat(i))) { // check if cmty hasn't already been added
        
        AddNode(Cmty.GetDat(i))); // add cmty

      }
      
      for (i -> j) { // edges
        
        if (Cmty.GetDat(i) != Cmty.GetDat(j)) { //  edge is between communities
          
          AddEdge(Cmty.GetDat(i), Cmty.GetDat(j), w); // incrementally "add edge" or "add to edge weight"
          
        }
        
      }
      
    }
    
    // -------
    
  }
  
}

} // namespace TSnap


#endif

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
void LouvainMethod(PGraph& Graph, TIntIntVH& CmtyVH) {
  
  // Variables needed
  
  Community Cmty; // or Quality (?)
  double best_improv;
  // ...
  
  while () { // quality improving
    
    // Phase 1 : MoveNode<Objective<Community> >(Graph, Q, NId)
    // -------
    
    while () { // quality improving
      
      // randomly permute nodes (?)
      
      for () { // nodes, randomly permuted
        
        if (Cmty.move_node_diff(NId, DstCmty) > best_improv) { // check if node move improves quality
          
          // update node move
          
        }
        
      }
      
      Cmty.move_node(NId, DstCmty); // execute node move with the best improvement in quality
      
    }
    
    // -------
    
    if () break; // break if not improved
    
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
    
  }
  
}

} // namespace TSnap


#endif

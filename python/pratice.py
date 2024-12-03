# Updated PDA to handle 1^x where x = 2 properly
import matplotlib.pyplot as plt
import networkx as nx

# Create a directed graph
G = nx.DiGraph()

# Add nodes with labels representing PDA states
G.add_node("q0", label="q0", shape='circle', style='filled', fillcolor='lightyellow')
G.add_node("q1", label="q1", shape='circle', style='filled', fillcolor='lightyellow')
G.add_node("q2", label="q2", shape='circle', style='filled', fillcolor='lightyellow')
G.add_node("q2'", label="q2'", shape='circle', style='filled', fillcolor='lightyellow')
G.add_node("q3", label="q3", shape='doublecircle', style='filled', fillcolor='lightyellow')

# Add edges with labels representing transitions
edges = [
    ("q0", "q0", "1, λ; Z"),  # First '1' handling, loop in q0
    ("q0", "q0", "1, Z; ZZ"),  # Second '1' handling, loop in q0
    ("q0", "q1", "λ, Z; Z"),  # Transition to q1 after finishing 1^x
    ("q1", "q1", "01, Z; ZZ"),  # Handles (01)^z in q1
    ("q1", "q2", "λ, Z; Z"),  # Transition to q2 when done with (01)^z
    ("q2", "q2", "1, Z; λ"),  # Handles 1^m in q2
    ("q2", "q2'", "λ, Z; λ"),  # Transition to q2' after finishing 1^m
    ("q2'", "q2'", "10, Z; λ"),  # Handles (10)^n in q2'
    ("q2'", "q3", "λ, Z; λ"),  # Final transition when stack is empty
]

# Add edges to graph
for (src, dst, label) in edges:
    G.add_edge(src, dst, label=label)

# Create layout for nodes
pos = {
    "q0": (0, 0),
    "q1": (2, 0),
    "q2": (4, 0.5),
    "q2'": (4, -0.5),
    "q3": (6, 0),
}

# Draw nodes with specific attributes
node_labels = nx.get_node_attributes(G, 'label')
nx.draw_networkx_nodes(G, pos, node_color='lightyellow', edgecolors='black', node_size=2000)
nx.draw_networkx_labels(G, pos, labels=node_labels, font_size=12, font_weight='bold')

# Draw edges with labels
edge_labels = {(src, dst): label for (src, dst, label) in edges}
nx.draw_networkx_edges(G, pos, edgelist=G.edges(), arrowstyle='-|>', arrowsize=20)
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_size=10)

# Hide axes
plt.axis('off')

# Display the graph
plt.title("Final PDA with Proper Handling of 1^x (x = 2)", fontsize=14, fontweight='bold')
plt.show()

def TreeEdges(prufer):
    m = len(prufer)
    vertices = m+2
    vertex_set = []
    tree = []
    
    # initialize the array of vertices
    for i in range(vertices):
        vertex_set.append(0)
        
    for i in range(vertices-2):
        vertex_set[prufer[i]-1] += 1
        
    # Find the smallest label not present in prufer
    for i in range(vertices-2):
        for j in range(vertices):
            if vertex_set[j] == 0:
                # store prufer set
                vertex_set[j] -= 1
                tree.append([j+1, prufer[i] ])
                vertex_set[prufer[i] - 1] -= 1;
                break
    
    j = 0;
    for i in range(vertices):
        if vertex_set[i] == 0 and j == 0:
            tree.append([i+1])
            j += 1
        elif vertex_set[i] == 0 and j == 1:
            tree[-1].append(i+1);
    return tree

# Reference: http://ide.geeksforgeeks.org/index.php

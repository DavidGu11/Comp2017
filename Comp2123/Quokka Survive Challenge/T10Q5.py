class Node:
    def __init__(self, name):
        self.name = name
        self.neighbours = []


naruto = Node('Naruto')
hinata = Node('Hinata')
sakura = Node('Sakura')
sasuke = Node('Sasuke')
shikamaru = Node('Shikamaru')
gaara = Node('Gaara')
temari = Node('Temari')
kankuro = Node('Kankuro')

naruto = Node('Naruto')
hinata = Node('Hinata')
sakura = Node('Sakura')
sasuke = Node('Sasuke')
shikamaru = Node('Shikamaru')
gaara = Node('Gaara')
temari = Node('Temari')
kankuro = Node('Kankuro')

naruto.neighbours.append(sasuke)
naruto.neighbours.append(sakura)
naruto.neighbours.append(hinata)
naruto.neighbours.append(shikamaru)
naruto.neighbours.append(gaara)

hinata.neighbours.append(naruto)

sasuke.neighbours.append(naruto)
sasuke.neighbours.append(sakura)

sakura.neighbours.append(sasuke)
sakura.neighbours.append(naruto)
sakura.neighbours.append(shikamaru)

shikamaru.neighbours.append(naruto)
shikamaru.neighbours.append(sakura)
shikamaru.neighbours.append(temari)

temari.neighbours.append(shikamaru)
temari.neighbours.append(gaara)
temari.neighbours.append(kankuro)

gaara.neighbours.append(naruto)
gaara.neighbours.append(temari)
gaara.neighbours.append(kankuro)

kankuro.neighbours.append(gaara)
kankuro.neighbours.append(temari)

# Use a global visited list, or you can follow T10Q4's method where
# you pass in the list directly into the function as a parameter.
visited = []


def DFS_recursive(node):
    global visited

    print(node.name)
    # input()
    for node in reversed(node.neighbours):
        if node.name not in visited:
            visited.append(node.name)
            DFS_recursive(node)


def BFS_iterative(node):
    # Local visited list, not the same as the global one.
    visited = [node.name]
    nodes_list = [node]

    while True:
        if len(nodes_list) == 0:
            break

        node = nodes_list[0]
        nodes_list = nodes_list[1:]

        print(node.name)

        for node in node.neighbours:
            if node.name not in visited:
                visited.append(node.name)
                nodes_list.append(node)


visited.append(naruto.name)
DFS_recursive(naruto)
print()
BFS_iterative(naruto)

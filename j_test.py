
import jdata as jd
import numpy as np
import tqdm

a={'str':'test','num':1.2,'list':[1.1,[2.1]],'nan':float('nan'),'np':np.arange(1,5,dtype=np.uint8)}
newdata=jd.load('MMC_Colin27_Atlas_Mesh_Version_2L.jmsh')

f_node = open('node.dat', 'w')
for i in tqdm.tqdm(newdata['MeshNode']):
    node = str(i[0]) + " " + str(i[1]) + " " + str(i[2]) + "\n"
    f_node.write(node)
f_node.close()

f_elm = open('element.dat', 'w')
for i in tqdm.tqdm(newdata['MeshElem']):
    element = str(i[0]) + " " + str(i[1]) + " " + str(i[2]) + " " + str(i[3]) + " " + str(i[4]) + "\n"
    f_elm.write(element)
f_elm.close()

f_face = open('MeshSurf.dat', 'w')
for i in tqdm.tqdm(newdata['MeshSurf']):
    element = str(int(i[0])) + " " + str(int(i[1])) + " " + str(int(i[2])) + " " + str(i[3]) + "\n"
    f_face.write(element)
f_face.close()
import glob
import cv2
import sklearn.cluster
import numpy as np

def vocabulaire(N, chemins, fichier, methode):
    descriptors = []
    sift = cv2.SIFT_create()
    for chemin in chemins:
        files = glob.glob('caltech/'+chemin+'/test/*.jpg')
        for file in files:
            image = cv2.imread(file)
            image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
            keys, descriptor = sift.detectAndCompute(image, None)
            descriptors.extend(descriptor)
    descriptors = np.array(descriptors)
    if(methode=='kmean'):
        cluster = sklearn.cluster.MiniBatchKMeans(n_clusters = N, random_state=0)
    cluster_index = cluster.fit_predict(descriptors)
    centers = cluster.cluster_centers_
    average_inertia = cluster.inertia_ / N
    error_max = max([np.linalg.norm(centers[element] - descriptors[index]) for index, element in enumerate(cluster_index)])
    if(fichier):
        np.savetxt(fichier, centers, delimiter=',')
    return average_inertia, error_max
        

chemins = ['cougar_face','crab','dolphin','dragonfly']
vocabulaire(10,chemins,'centers.txt','kmean')
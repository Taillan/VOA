import glob
import cv2
import sklearn.cluster
import numpy as np
import matplotlib.pyplot as plt

N_MAX = 40
CHEMINS = ['cougar_face','crab','dolphin','dragonfly']
PLOT_NAME = "plot.png"


def vocabulaire(N, chemins, fichier, methode):
    descriptors = []
    sift = cv2.SIFT_create()
    for chemin in chemins:
        files = glob.glob('asset/'+chemin+'/test/*.jpg')
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
        
def coude(chemins):
    variances = []
    errors = []
    clusters = []
    
    for n in range(1, N_MAX, 10):
        print("Calculating cluster : " + str(n) + " / " + str(N_MAX) + "...")
        clusters.append(n)
        average, error = vocabulaire(n,chemins,None,'kmean')
        variances.append(average)
        errors.append(error)

    fig, axs = plt.subplots(2, figsize=(20, 10))
    axs[0].plot(clusters, variances)
    axs[0].set_title("Average Variance")
    axs[0].set_xlabel("Clusters")
    axs[1].plot(clusters, errors)
    axs[1].set_title("Error Max")
    axs[1].set_xlabel("Clusters")
    plt.savefig('plot.png')
    print("Plot save as " + PLOT_NAME)

def vectoriser(im, voca):
    sift = cv2.SIFT_create()
    image = cv2.imread(im)
    image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    keys, descriptors = sift.detectAndCompute(image, None)

    descriptors_list = np.array()
    descriptors_list = np.append(descriptors_list, descriptors)

    descriptors = np.reshape(descriptors_list, (len(descriptors_list) // 64, 64))
    descriptors = np.float32(descriptors)

    return vocabulaire.predict(descriptors)

coude(CHEMINS)
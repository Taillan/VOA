# echo.py

import sklearn.cluster
import sys
import glob
import cv2
import numpy as np
 

def vocabulaire(N,chemins,fichier,methode):
    imListe = glob.glob('asset/*/test/*.jpg')
    descriptors = []
    #imListe = glob.glob('asset/*/*.jpg')
    s = cv2.SIFT_create()
    error_max = 0

    print("SIIFT Created")
    for el in imListe:
        image = cv2.imread(el)
        image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        keys,descriptor = s.detectAndCompute(image_gray,None)
        descriptors.extend(descriptor)
    
    descriptors = np.array(descriptors)
    centroid,label,inertia = sklearn.cluster.k_means(descriptors, N)
    print("centroid =", centroid )
    print("label =", label )
    print("inertia =", inertia )
    np.savetxt(fichier,centroid,fmt='%10.5f')
    inertia_moy = inertia/N
    
    for ine in label:
        error = abs(ine - inertia_moy)
        if(error > error_max):
            error_max = error
    
    return inertia_moy,error_max

def main():
    print("start")
    error = vocabulaire(10,"asset/","matrice_vocabulaire.txt","methode")
    print(error)

if __name__ == '__main__':
    sys.exit(main())
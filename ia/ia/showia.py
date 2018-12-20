import numpy as np
import cv2


import tensorflow as tf
from tensorflow import keras
import os
import sys
import cv2
import time
import glob
import matplotlib.pyplot as plt


# Recoit le nom du fichier qui contient notre context TvShow a utilisé





# Charge les informations et crée ma pipeline de données
image_size = (64, 64) # image size 64x64

def LoadImagesFrom(folder):
    images = []
    for filename in glob.glob(folder):
        image = cv2.imread(filename, 0) # read image as grayscale
        image = cv2.imread(filename)
        if(image is not None and image.data):
            image = cv2.resize(image, image_size)
            image = cv2.normalize(image, None, alpha=0, beta=1, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_32F)
            images.append(image)
    return images

def RoundDatasets(cats, dogs):
    count_cats = len(cats)
    count_dogs = len(dogs)

    print('total cat images:', count_cats)
    print('total dog images:', count_dogs)

    min_images = min(count_cats, count_dogs)
    round_min_images = min_images % 10
    min_images = min_images - round_min_images

    diff_images_cats = count_cats - min_images
    diff_images_dogs = count_dogs - min_images

    del cats[-diff_images_cats:]
    del dogs[-diff_images_dogs:]

    return cat_images, dog_images, min_images

def SplitDataset(cats, dogs, length_dataset, percentage):
    length_test_images = int(length_dataset / percentage)

    cats_test = cats[-length_test_images:]
    cats_test_label = np.empty(length_test_images, int)
    cats_test_label.fill(0)

    dogs_test = dogs[-length_test_images:]
    dogs_test_label = np.empty(length_test_images, int)
    dogs_test_label.fill(1)

    test_images = cats_test + dogs_test
    test_labels = np.concatenate([cats_test_label, dogs_test_label])

    del cats[-length_test_images:]
    cats_train_label = np.empty(len(cats), int)
    cats_train_label.fill(0)

    del dogs[-length_test_images:]
    dogs_train_label = np.empty(len(dogs), int)
    dogs_train_label.fill(1)

    train_images = cats + dogs
    train_labels = np.concatenate([cats_train_label, dogs_train_label])

    return (train_images, train_labels), (test_images, test_labels)

def ShuffleDataset(images, labels):
    images_shuffle = []
    labels_shuffle = []
    indices = np.arange(len(images))
    np.random.shuffle(indices)
    for i in indices:
        images_shuffle.append(images[i])
        labels_shuffle.append(labels[i])
    return images_shuffle, labels_shuffle

def plot_image(i, predictions_array, true_label, img):
    predictions_array, true_label, img = predictions_array[i], true_label[i], img[i]
    plt.grid(False)
    plt.xticks([])
    plt.yticks([])

    plt.imshow(img, cmap=plt.cm.binary)

    predicted_label = np.argmax(predictions_array)
    if predicted_label == true_label:
        color = 'blue'
    else:
        color = 'red'

    plt.xlabel("{} {:2.0f}% ({})".format(class_names[predicted_label],
                                         100*np.max(predictions_array),
                                         class_names[true_label]),
               color=color)

def plot_value_array(i, predictions_array, true_label):
    predictions_array, true_label = predictions_array[i], true_label[i]
    plt.grid(False)
    plt.xticks([])
    plt.yticks([])
    thisplot = plt.bar(range(2), predictions_array, color="#777777")
    plt.ylim([0, 1])
    predicted_label = np.argmax(predictions_array)

    thisplot[predicted_label].set_color('red')
    thisplot[true_label].set_color('blue')


cat_images = LoadImagesFrom('/home/wq/t1/dwight/*.png')
dog_images = LoadImagesFrom('/home/wq/t1/jim/*.png')

class_names = ['cat', 'dog']

cat_images, dog_images, length_dataset = RoundDatasets(cat_images, dog_images)
#length_dataset = 42
(train_images, train_labels), (test_images, test_labels) = SplitDataset(cat_images, dog_images, length_dataset, 10)

train_images, train_labels = ShuffleDataset(train_images, train_labels)
test_images, test_labels = ShuffleDataset(test_images, test_labels)

# convert to numpy array
train_images = np.asarray(train_images)
train_labels = np.asarray(train_labels)
test_images = np.asarray(test_images)
test_labels = np.asarray(test_labels)


model = keras.Sequential([
    keras.layers.Conv2D(32, (3, 3), input_shape= (64, 64, 3), activation = 'relu'),
    keras.layers.MaxPooling2D(pool_size = (2,2)),
    keras.layers.Conv2D(32, (3, 3), activation = 'relu'),
    keras.layers.MaxPooling2D(pool_size = (2, 2)),
    keras.layers.Flatten(),
    keras.layers.Dense(units = 128, activation = 'relu'),
    keras.layers.Dense(units = 2, activation = 'sigmoid')
])

model.summary()

model.compile(optimizer=tf.train.AdamOptimizer(),
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])
model.fit(train_images, train_labels, epochs=5)

test_loss, test_acc = model.evaluate(test_images, test_labels)

print('Test accuracy:', test_acc)
predictions = model.predict(test_images)

num_rows = 5
num_cols = 3
num_images = num_rows*num_cols
plt.figure(figsize=(2*2*num_cols, 2*num_rows))
for i in range(8):
    plt.subplot(num_rows, 2*num_cols, 2*i+1)
    plot_image(i, predictions, test_labels, test_images)
    plt.subplot(num_rows, 2*num_cols, 2*i+2)
    plot_value_array(i, predictions, test_labels)

plt.show()
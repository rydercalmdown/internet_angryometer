import nltk
from nltk.tokenize import word_tokenize
from helpers import remove_noise
import pickle


nltk.data.path.append('/root/nltk_data')
classifier_file = open('classifier.pickle', 'rb')
classifier_obj = pickle.load(classifier_file)


def classify(sentence):
    custom_tokens = remove_noise(word_tokenize(sentence))
    return classifier_obj.classify(dict([token, True] for token in custom_tokens))

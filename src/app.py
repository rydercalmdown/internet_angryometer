from flask import Flask, jsonify, request
from classifier import classify
from twitter_handler import get_recent_tweets


app = Flask(__name__)


def get_current_mood_percent():
    """Gets the current mood of the internet as a percent"""
    tweets = get_recent_tweets()
    positive_tweets = 0
    negative_tweets = 0
    for t in tweets:
        if classify(t) == 'Positive':
            positive_tweets += 1
        else:
            negative_tweets += 1
    return negative_tweets / ( negative_tweets + positive_tweets )


def convert_to_degress(percent):
    """Converts percent to degrees negativity"""
    return int(percent * 180)


@app.route('/degrees/')
def degrees():
    """View for degrees"""
    return str(convert_to_degress(get_current_mood_percent()))


@app.route('/percent/')
def percent():
    """View for percent"""
    return str(int(get_current_mood_percent() * 100))


@app.route('/')
def index():
    """View for index"""
    return jsonify({'status': 'ok'})


if __name__ == '__main__':
    app.run(debug=False, host='0.0.0.0')

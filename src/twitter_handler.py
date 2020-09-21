import os
from urllib.parse import urlencode
import twitter


twitter_api = twitter.Api(
    consumer_key=os.environ['CONSUMER_KEY'],
    consumer_secret=os.environ['CONSUMER_SECRET'],
    access_token_key=os.environ['ACCESS_TOKEN_KEY'],
    access_token_secret=os.environ['ACCESS_TOKEN_SECRET']
)

def get_recent_tweets():
    """Returns a list of recent tweets"""
    query_phrase = 'im%20feeling'
    params = {
        'q': query_phrase,
        'src': 'typed_query',
        'f': 'live',
    }
    results = twitter_api.GetSearch(raw_query=urlencode(params))
    return [r.text for r in results]

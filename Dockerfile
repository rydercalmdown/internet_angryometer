FROM python:3.7
WORKDIR /code
COPY src/requirements.txt .
COPY src/download_nltk_components.py .
RUN pip install -r requirements.txt && python download_nltk_components.py
COPY src .
ENTRYPOINT [ "/bin/bash" ]
CMD [ "entrypoint.sh" ]

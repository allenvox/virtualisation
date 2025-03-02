from flask import Flask
import random
app = Flask(__name__)

@app.route('/')
def random_number():
    return str(random.randint(1,100))

@app.route('/health')
def health_check():
    return "OK", 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000)

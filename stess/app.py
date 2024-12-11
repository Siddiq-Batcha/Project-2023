from flask import Flask, url_for, render_template, request, redirect, flash
import joblib as jb
import numpy as np
import pandas as pd
import model_class as md
import model_train as mt

app = Flask(__name__)

model = jb.load("stress_predictor")

@app.route('/')
def hello_world():
    return render_template('index.html')


@app.route('/predict',methods = ['POST', 'GET'])
def score_predict():
    inputs = []
    inputs.append(request.form["temperature"])
    inputs.append(request.form["blood"])
    data = np.array([inputs])
    data_df = pd.DataFrame(data, columns = mt.X_train.columns)
    data_transform = md.final_pipeline.fit_transform(data_df)
    result = model.predict(data_transform)
    if result[0][0]>35 and result[0][1]>120:
        output='1'
    else:
        output='0'
    return render_template("index.html", pred = output )


if __name__ == '__main__':
   app.run(debug=True)

   
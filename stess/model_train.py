import model_class as md

# Preparing training data and dumping the model

data = md.pd.read_csv("port.csv")

split = md.DataSplitting(data)

train_data, test_data = split.train_test_split(data,0.5,1000)

X_train, Y_train = split.feature_label_split("output", train_data)

X_prepared = md.final_pipeline.fit_transform(X_train)

model = md.LinearRegression()

model.fit(X_prepared,X_train)

md.dump(model, "stress_predictor")

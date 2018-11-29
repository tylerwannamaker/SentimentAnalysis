from sklearn.feature_extraction.text import CountVectorizer
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression
import random
data = []
data_labels = []
with open("./new_review_output_py_pos.txt") as f:
    for i in f:
        data.append(i)
        data_labels.append('pos')

with open("./new_review_output_py_neg.txt") as f:
    for i in f:
        data.append(i)
        data_labels.append('neg')
vectorizer = CountVectorizer(
    analyzer = 'word',
    lowercase = False,
)
features = vectorizer.fit_transform(
    data
)
features_nd = features.toarray()

X_train, X_test, y_train, y_test = train_test_split(
        features_nd,
        data_labels,
        train_size=0.7,
        random_state=1234)
log_model = LogisticRegression()
log_model = log_model.fit(X=X_train, y=y_train)
y_pred = log_model.predict(X_test)
j = random.randint(0,len(X_test)-7)
for i in range(j,j+7):
    ind = features_nd.tolist().index(X_test[i].tolist())
from sklearn.metrics import accuracy_score
print("Accuracy: ")
print(accuracy_score(y_test, y_pred))
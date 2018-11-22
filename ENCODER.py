

import tensorflow as tf
import numpy as np



# Configure Here
num_iters = 2
learning_rate = 0.01
batch_size = 1
num_batch = 20000000

# Configure Network
num_layer_1 = 32
num_layer_2 = 16
num_layer_3 = 20


data_file = open("DATA.txt")
anchor_file = open("ANCHOR.txt")
close_file = open("CLOSE.txt")
away_file = open("AWAY.txt")

raw_data = []
tmp_count = 0;

for lin in data_file.readlines():
	raw_data.append(float(lin))
	tmp_count = tmp_count + 1
	if tmp_count == 60000 * 64:
		break
		
# Exchange

for i in range(10000):
	for j in range(8):
		tmp_i = raw_data[i * 64 * 5 + j]
		raw_data[i * 64 * 5 + j] = raw_data[50000 * 64 + i * 64 + j]
		raw_data[50000 * 64 + i * 64 + j] = tmp_i


raw_ANCHOR = []
raw_CLOSE = []
raw_AWAY = []

for lin in anchor_file.readlines():
	raw_ANCHOR.append(int(lin))
for lin in close_file.readlines():
	raw_CLOSE.append(int(lin))
for lin in away_file.readlines():
	raw_AWAY.append(int(lin))
	
	

ANCHOR_PNT = tf.placeholder("float", [None, 64])
CLOSE_PNT = tf.placeholder("float", [None, 64])
AWAY_PNT = tf.placeholder("float", [None, 64])

weights = {
	'encoder_1': tf.Variable(tf.random_normal([64, num_layer_1])),
	'encoder_2': tf.Variable(tf.random_normal([num_layer_1, num_layer_2])),
	'encoder_3': tf.Variable(tf.random_normal([num_layer_2, num_layer_3])),
}

biases = {
	'bias_1': tf.Variable(tf.random_normal([num_layer_1])),
	'bias_2': tf.Variable(tf.random_normal([num_layer_2])),
	'bias_3': tf.Variable(tf.random_normal([num_layer_3])),
}


def encoder_model(x):
	layer_1 = tf.nn.sigmoid(tf.add(tf.matmul(x, weights['encoder_1']), biases['bias_1']))
	layer_2 = tf.nn.sigmoid(tf.add(tf.matmul(layer_1, weights['encoder_2']), biases['bias_2']))
	layer_3 = tf.nn.sigmoid(tf.add(tf.matmul(layer_2, weights['encoder_3']), biases['bias_3']))
	return layer_3
	
MODEL_ANCHOR = encoder_model(ANCHOR_PNT)
MODEL_CLOSE = encoder_model(CLOSE_PNT)
MODEL_AWAY = encoder_model(AWAY_PNT)

target = tf.sigmoid(tf.pow(MODEL_ANCHOR - MODEL_CLOSE, 2) - tf.pow(MODEL_ANCHOR - MODEL_AWAY, 2))

loss = tf.reduce_mean(target)

optimizer = tf.train.RMSPropOptimizer(learning_rate).minimize(loss)

init = tf.global_variables_initializer()

with tf.Session() as sess:
	
	sess.run(init)
	
	# Training
	'''
	tmp_count = 0
	for z in range(num_iters):
		for i in range(0, 60000, 100):
			for j in range(i + 100, 60000, 100):
				for k in range(j + 100, 60000, 100):
					
					tmp_data_anchor = [[]]
					tmp_data_close = [[]]
					tmp_data_away = [[]]
					
					if raw_CAA[tmp_count] == 1:
						# j is close
						for m in range(64):
							tmp_data_anchor[0].append(raw_data[i * 64 + m])
							tmp_data_close[0].append(raw_data[j * 64 + m])
							tmp_data_away[0].append(raw_data[k * 64 + m])
					else:
						for m in range(64):
							tmp_data_anchor[0].append(raw_data[i * 64 + m])
							tmp_data_close[0].append(raw_data[k * 64 + m])
							tmp_data_away[0].append(raw_data[j * 64 + m])
						
					
					_, l = sess.run([optimizer, loss], feed_dict = {ANCHOR_PNT: tmp_data_anchor, CLOSE_PNT: tmp_data_close, AWAY_PNT: tmp_data_away})
					
					if k % 1000 == 0:
						print('Step %i: Minibatch Loss: %f' % (i, l))
	'''
	for z in range(num_batch):
		
		data_ANCHOR = []
		data_CLOSE = []
		data_AWAY = []
		for j in range(batch_size):
			data_ANCHOR.append([])
			data_CLOSE.append([])
			data_AWAY.append([])
			for k in range(64):
				data_ANCHOR[j].append(raw_data[raw_ANCHOR[z * batch_size + j] * 64 + k])
				data_CLOSE[j].append(raw_data[raw_CLOSE[z * batch_size + j] * 64 + k])
				data_AWAY[j].append(raw_data[raw_AWAY[z * batch_size + j] * 64 + k])
		
		_, l = sess.run([optimizer, loss], feed_dict = {ANCHOR_PNT: data_ANCHOR, CLOSE_PNT: data_CLOSE, AWAY_PNT: data_AWAY})
		if z % 10000 == 0:
			print('Step %i: Minibatch Loss: %f' % (z, l))
		
		
	for i in range(60000):
		tmp_data = [[]]
		for j in range(64):
			tmp_data[0].append(raw_data[i * 64 + j])
		g = sess.run(MODEL_ANCHOR, feed_dict = {ANCHOR_PNT: tmp_data})
		print(g)
		
	











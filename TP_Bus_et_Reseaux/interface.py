from unicodedata import category
from flask import (Flask, json, jsonify, abort, render_template, request)
from http import HTTPStatus
import serial
ser=serial.Serial('/dev/ttyAMA0')
ser.baudrate=115200
#ser.port='COM1'
app = Flask(__name__)

#recipes = [
#    {
#        'id': 1,
#        'name': 'Egg Salad',
#        'description': 'This is a lovely egg salad recipe.'
#    },
#    {
#        'id': 2, 'name': 'Tomato Pasta',
#        'description': 'This is a lovely tomato pasta recipe.'
#    }
#]

@app.route('/')
def hello_world():
	print("Hello world")
	return 'Hello, World!\n'

welcome_sentence = "Welcome to 3ESE API!"
temperature_sentence = "temperature ="
temp0 = 0
temp1 = 1
temp=temp0

@app.route('/api/welcome/', methods=['GET'])
def api_retrieve():
    return jsonify({'data': welcome_sentence}), HTTPStatus.OK 

# @app.route('/api/welcome/', methods=['POST'])
# def api_create():
     
@app.route('/api/welcome/<int:letter_index>', methods=['GET'])
def api_retrieve_letter(letter_index):
    if(letter_index>len(welcome_sentence)):
        abort(HTTPStatus.NOT_FOUND)
    if request.method == 'GET':
        return jsonify({'data': welcome_sentence[letter_index]}), HTTPStatus.OK
	
# @app.route('/api/temp/')
# def temperature():
#	return temperature_sentence,temp

@app.route('/api/temp/', methods=['POST','GET'])
def api_update_temp(path=None):
	if request.method == 'POST':
		data = request.get_json(force=True, silent=True, cache=True)
		print("AVANT Get_t part")
		print(data)
		commande = data["name"]
		if(commande=="GET_T"):
			print("Get_t part")
			global temp
			temp = get_t()
			# temp = temp1
	return jsonify({'message':temperature_sentence,'data':temp}),HTTPStatus.CREATED
	
#@app.route('/api/temp/', methods=['GET'])
#def api_retrieve_temp():
#    return jsonify(message=temperature_sentence,
#				   data=temp), HTTPStatus.OK
    
#@app.route('/api/welcome/<int:word_index>',methods=['PUT'])
#def api_update_word(word_index):
#     # data = request.get_json(force=True, silent=True, cache=True)
#	data = request.get_json(force=True, silent=True, cache=True)
#	word=data.get('word')
#	sentence_list = list(sentence.split(" "))
#	sentence_list.insert(word_index,word)
#	sentence = " ".join(sentence_list)
#	return jsonify({'data':sentence})

@app.errorhandler(HTTPStatus.NOT_FOUND)
def page_not_found(error):
    return render_template('page_not_found.html'), HTTPStatus.NOT_FOUND



def get_t():
	ser.open()
	if (ser.is_open==True):
		ser.write("GET_T")
	else:
		print("Error with ser.open")
		return(-1)
	str_temp=ser.readline()
	#ser.close()
	return float(str_temp[3:8])

def get_p():
	#ser.open()
	if (ser.is_open==True):
		ser.write("GET_P")
	else:
		print("Error with ser.open")
		return(-1)
	str_press=ser.readline()
	#ser.close()
	press=float(str_press[2:8])
	return(press)

def set_k(k):
	#ser.open()
	if (ser.is_open==True):
		ser.write("SET_K="+str(k))
	else:
		print("Error with ser.open")
		return -1
	check=ser.readline()
	if (check=="SET_K=OK"):
		#ser.close()
		return 0
	else:
		print("Error check")
		return -2

def get_k():
	#ser.open()
	if (ser.is_open==True):
		ser.write("GET_K")
	else:
		print("Error with ser.open")
		return(-1)
	str_k=ser.readline()
	#ser.close()
	k=float(str_k[2:10])
	return(k)

def get_a():
	#ser.open()
	if (ser.is_open==True):
		ser.write("GET_A")
	else:
		print("Error with ser.open")
		return(-1)
	str_a=ser.readline()
	#ser.close()
	a=float(str_a[2:10])
	return(a)
from unicodedata import category
from flask import (Flask, json, jsonify, abort, render_template, request)
import serial

# Setup of the serial port of the Raspberry Pi
ser=serial.Serial('/dev/ttyAMA0') 
ser.baudrate=115200
#ser.port='COM1'

# server setup, web pages definition
app = Flask(__name__)

@app.route('/')
def hello_world():
	return 'Hello, World!\n'

welcome_sentence = "Welcome to 3ESE API!"
temperature_sentence = "temperature ="
temp0 = 0 # variables for test
temp1 = 1
temp=temp0

# Each of the following function enables to modify the sentence printed on our web page which is loaded on the server (the Raspberry Pi Zero)

# This function enables to modify the whole sentence
@app.route('/api/welcome/<string:welcome_sentence>', methods=['POST'])
def api_welcome_sentence(sentence):
	global welcome_sentence
	if request.method == 'POST':
		welcome_sentence=sentence
		return jsonify({'data': welcome_sentence})
	
# This function enables to retrieve the sentence or to delete it

@app.route('/api/welcome/', methods=['GET','DELETE'])
def api_welcome():
	global welcome_sentence
	if request.method=='GET':
		return jsonify({'data': welcome_sentence})
	if request.method=='DELETE':
		welcome_sentence = ''
		return jsonify({'data': welcome_sentence})
	
# This function enables to retrieve a letter at a specific index or to delete this letter 
     
@app.route('/api/welcome/<int:letter_index>', methods=['GET','DELETE'])
def api_welcome_index(letter_index):
	global welcome_sentence
	letters_list = list(welcome_sentence)
	if request.method == 'GET':
		if(letter_index<len(letters_list)):
			return jsonify({'data': welcome_sentence[letter_index]})
		else:
			abort(404)
	if request.method == 'DELETE':
		letters_list[letter_index]=""
		welcome_sentence="".join(letters_list)
		return jsonify({'data': welcome_sentence})
	
# This function enables to insert a word at a specific position
	
@app.route('/api/welcome/<int:word_index>/<string:word>',methods=['PUT'])
def api_welcome_word(word_index,word):
	global welcome_sentence
	words_list = list(welcome_sentence.split(" "))
	words_list.insert(word_index,word)
	welcome_sentence = " ".join(words_list)
	return jsonify({'data':welcome_sentence})

# This function enables to modify a letter at a specific index
	
@app.route('/api/welcome/<int:letter_index>/<string:letter>',methods=['PATCH'])
def api_welcome_letter(letter_index,letter):
	global welcome_sentence
	letters_list=list(welcome_sentence)
	letters_list[letter_index]=letter
	welcome_sentence="".join(letters_list)
	return welcome_sentence

# This function enables to retrieve the temperature. It seems not to work.

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
	return jsonify({'message':temperature_sentence,'data':temp}),201


# This function enables to redirect the user towards a customized Error 404 web page
@app.errorhandler(404)
def page_not_found(error):
    return render_template('page_not_found.html'), 404

# All the functions below enable to retrieve the temperature, the pressure, the angle, the K factor (precision factor) as well as to modify this factor.
# We have not had time to use these functions in the "flask" ones except for get_t but we let them here to show the way we proceeded.

def get_t():
	ser.open()
	if (ser.is_open==True):
		ser.write("GET_T") # Give me the temperature
	else:
		print("Error with ser.open")
		return(-1)
	str_temp=ser.readline()
	#ser.close() 
	return float(str_temp[3:8]) # The temperature respects the format T=+12.50_C

def get_p():
	#ser.open()
	if (ser.is_open==True):
		ser.write("GET_P")
	else:
		print("Error with ser.open")
		return(-1)
	str_press=ser.readline()
	#ser.close()
	press=float(str_press[2:8]) # The pressure respects the format P=102300Pa
	return(press)

def set_k(k):
	#ser.open()
	if (ser.is_open==True):
		ser.write("SET_K="+str(k)) # Format : SET_K=1234
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
	k=float(str_k[2:10]) # Format : K=12.34000
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
	a=float(str_a[2:10]) # Format : A=125.7000
	return(a)
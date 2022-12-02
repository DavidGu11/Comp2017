# Importing the Flask Framework

from modules import *
from flask import *
import database
import configparser
import random
import copy


page = {}
session = {}


# Initialise the FLASK application
app = Flask(__name__)
app.secret_key = 'SoMeSeCrEtKeYhErE'


# Debug = true if you want debug output on error ; change to false if you dont
app.debug = True
veri_list = []

unikey = None
name = None

current_sid = None
current_pass = None
veri = random.randint(1000,9999)
# Read my unikey to show me a personalised app
config = configparser.ConfigParser()
config.read('config.ini')
my_unikey = config['DATABASE']['user']
portchoice = config['FLASK']['port']

#####################################################
##  INDEX
#####################################################

# What happens when we go to our website
@app.route('/')
def index():
    # If the user is not logged in, then make them go to the login page
    if( 'logged_in' not in session or not session['logged_in']):
        return redirect(url_for('login'))
    page['unikey'] = unikey
    page['title'] = 'Welcome'
    return render_template('welcome.html', session=session, page=page)

################################################################################
# Login Page
################################################################################

# This is for the login
# Look at the methods [post, get] that corresponds with form actions etc.
@app.route('/login', methods=['POST', 'GET'])
def login():
    global unikey
    global current_sid
    global current_pass
    global veri
    page = {'title' : 'Login', 'unikey' : unikey}
    # If it's a post method handle it nicely
    veri = random.randint(1000,9999)
    veri_list.append(veri)
    flash("verification code: " + str(veri_list[0]))
    if(request.method == 'POST'):
        # Get our login value
      
        
        if str(veri_list[0]) != str(request.form['Verification']):
#             flash("x")
            flash("verification code was: " + str(veri_list[0]) + ", you entered: " + request.form['Verification'] + ". Pls retype the code shown below")
            veri_list[0] = random.randint(1000,9999)
     
            
            return redirect(url_for('login'))
            
        current_sid = request.form['sid']
        current_pass = request.form['password']
        
        current_sid = current_sid.replace("'","''")
        current_pass = current_pass.replace("'","''")
        val = database.check_login(current_sid, current_pass)
        

        # If our database connection gave back an error
        if(val == None):
            flash("""Error with the database connection. Please check your terminal
            and make sure you updated your INI files.""")
            return redirect(url_for('login'))

        # If it's null, or nothing came up, flash a message saying error
        # And make them go back to the login screen
        if(val is None or len(val) < 1):
            flash('There was an error logging you in')
            return redirect(url_for('login'))
        # If it was successful, then we can log them in :)
        session['name'] = val[1]
        session['sid'] = request.form['sid']
        session['logged_in'] = True
        return redirect(url_for('index'))
    else:
        # Else, they're just looking at the page :)
        if('logged_in' in session and session['logged_in'] == True):
            return redirect(url_for('index'))
        return render_template('index.html', page=page)


################################################################################
# Logout Endpoint
################################################################################

@app.route('/logout')
def logout():
    session['logged_in'] = False
    flash('You have been logged out')
    return redirect(url_for('index'))


################################################################################
# Classroom Page
################################################################################

@app.route('/list-classrooms')
def classrooms():
    classrooms = database.get_classroom()
    # TODO
    # Now it's your turn to add to this ;)
    # Good luck!
    #   Look at the function below
    #   Look at database.py
    #   Look at units.html and transcript.html
    if (classrooms is None):
        # Set it to an empty list and show error message
        classrooms = []
        flash('Error, there are no classrooms')
    page['title'] = 'classrooms'
    return render_template('classrooms.html', page=page, session=session,classrooms = classrooms)


@app.route('/search_classrooms',methods = ['POST','GET'])
def search_classroom():
    seats = 0
    seats = request.values.get("search_classrooms")
    
    classroom = database.search_classroom(seats)
    print(seats)

    # TODO
    # Now it's your turn to add to this ;)
    # Good luck!
    #   Look at the function below
    #   Look at database.py
    #   Look at units.html and transcript.html
    if (classroom is None):
        # Set it to an empty list and show error message
        classroom = []
        flash(seats)
    page['title'] = 'classrooms search'
    return render_template('search_classrooms.html', page=page, session=session,classrooms = classroom)



@app.route('/classrooms_type')
def classrooms_type():
    classroom_type = None
    classroom_type = database.classroom_type()
    # TODO
    # Now it's your turn to add to this ;)
    # Good luck!
    #   Look at the function below
    #   Look at database.py
    #   Look at units.html and transcript.html
    if (classroom_type  is None):
        # Set it to an empty list and show error message
        classroom_type  = []
        flash('Error, there are no classrooms')
    page['title'] = 'classroom type'
    return render_template('classrooms_type.html', page=page, session=session,classrooms = classroom_type)


@app.route('/classrooms_add',methods = ['POST','GET'])
def classrooms_add():
    newmov = {}
    newmov['seats'] = 0
    add_classrooms = None
    if(request.method == 'POST'):
        if('classroomid' not in request.form):
            newmov['classroomid'] = 'Empty id'
        else:
            newmov['classroomid'] = request.form['classroomid']
            print("value is ",newmov['classroomid'])
            
        if('seats' not in request.form):
            newmov['seats'] = 'Empty id'
        else:
            newmov['seats'] = request.form['seats']
            print("value is ",newmov['seats'])
            
        if('type' not in request.form):
            newmov['type'] = 'Empty id'
        else:
            newmov['type'] = request.form['type']
            print("value is ",newmov['type'])
  
            
        add_classrooms = database.add_classroom(newmov['classroomid'],newmov['seats'],newmov['type'])

    if (add_classrooms is None):
        # Set it to an empty list and show error message
        add_classrooms = []
        flash("no class")
    page['title'] = 'add classroom'
    return render_template('classrooms_add.html', page=page, session=session,classrooms = add_classrooms)


################################################################################
# Assessment Page
################################################################################

@app.route('/list-assessments')
def assessments():
    assessments = database.get_assessments()
    # TODO
    # Now it's your turn to add to this ;)
    # Good luck!
    #   Look at the function below
    #   Look at database.py
    #   Look at units.html and transcript.html

    if (assessments is None):
        # Set it to an empty list and show error message
        classrooms = []
        val = database.create_assessments()
        flash(val)
    page['title'] = 'classrooms'
    return render_template('Assessments.html', page=page, session=session,assessments = assessments)


@app.route('/search_assessments',methods = ['POST','GET'])
def search_assessments():
    unit = None
    unit = request.values.get("search_assessments")
    
    assessment = database.search_assessments(unit)

    # TODO
    # Now it's your turn to add to this ;)
    # Good luck!
    #   Look at the function below
    #   Look at database.py
    #   Look at units.html and transcript.html
    if (assessment is None):
        # Set it to an empty list and show error message
        assessment = []
        flash(unit)
    page['title'] = 'unit search'
    return render_template('search_assessments.html', page=page, session=session,assessments = assessment)



@app.route('/add_assessments',methods = ['POST','GET'])
def add_assessments():
    newmov = {}
    newmov['seats'] = 0
    add_assessment = None
    if(request.method == 'POST'):
            
        if('Ass_ID' not in request.form):
            newmov['Ass_ID'] = 'Empty id'
        else:
            newmov['Ass_ID'] = request.form['Ass_ID']
            print("value is ",newmov['Ass_ID'])
            
        if('uoSCode' not in request.form):
            newmov['uoSCode'] = 'Empty id'
        else:
            newmov['uoSCode'] = request.form['uoSCode']
            print("value is ",newmov['uoSCode'])
            
        if('Semester' not in request.form):
            newmov['Semester'] = 'Empty id'
        else:
            newmov['Semester'] = request.form['Semester']
            print("value is ",newmov['Semester'])
       
        if('year' not in request.form):
            newmov['year'] = 'Empty id'
        else:
            newmov['year'] = request.form['year']
            print("value is ",newmov['year'])
            
        if('weighting' not in request.form):
            newmov['weighting'] = 'Empty id'
        else:
            newmov['weighting'] = request.form['weighting']
            print("value is ",newmov['weighting'])
            
            
  
            
        add_assessment = database.add_assessments(newmov['Ass_ID'],newmov['uoSCode'],newmov['Semester'],newmov['year'],newmov['weighting'])

    if (add_assessment is None):
        # Set it to an empty list and show error message
        add_assessment = []
        flash("no ass")
    page['title'] = 'add_assessment'
    return render_template('add_assessments.html', page=page, session=session,assessment = add_assessment)

################################################################################
# Update password and address page
################################################################################

@app.route('/Update_Address',methods = ['POST','GET'])
def update_address():
    add_address = None
    if(request.method == 'POST'):
            
            
        add_address = database.Update_Address(request.form['Address'],current_sid)

    if (add_address is None):
        # Set it to an empty list and show error message
        add_address = []
        flash("no address")
    page['title'] = 'add_address'
    return render_template('Update_Address.html', page=page, session=session,assessment = add_address)

@app.route('/Update_Password',methods = ['POST','GET'])
def update_password():
    passw = None
    hind = 0
    if(request.method == 'POST'):
            
        old_pass = request.form['old_Password']
        if old_pass == current_pass:
            passw = database.Update_Password(request.form['Password'],current_sid)
            hind += 1
        else:
            flash("Your old password is wrong, please try again")

    if (passw is None and hind == 0):
        # Set it to an empty list and show error message
        passw = []
        flash("No password")
    page['title'] = 'update_password'
    return render_template('Update_Password.html', page=page, session=session,assessment = passw)




#!/usr/bin/env python3

from modules import pg8000
import configparser


################################################################################
# Connect to the database
#   - This function reads the config file and tries to connect
#   - This is the main "connection" function used to set up our connection
################################################################################

def database_connect():
    # Read the config file
    config = configparser.ConfigParser()
    config.read('config.ini')

    # Create a connection to the database
    connection = None
    try:
        '''
        This is doing a couple of things in the back
        what it is doing is:

        connect(database='y12i2120_unikey',
            host='soit-db-pro-2.ucc.usyd.edu.au,
            password='password_from_config',
            user='y19i2120_unikey')
        '''
        
        data_owner = 'y22s2i2120_zigu7311'
        connection = pg8000.connect(database='y22s2i2120_zigu7311',
                                    user=config['DATABASE']['user'],
                                    password=config['DATABASE']['password'],
                                    host=config['DATABASE']['host'])
    except pg8000.OperationalError as e:
        print("""Error, you haven't updated your config.ini or you have a bad
        connection, please try again. (Update your files first, then check
        internet connection)
        """)
        print(e)
    except pg8000.ProgrammingError as e:
        print("""Error, config file incorrect: check your password and username""")
        print(e)
    except Exception as e:
        print(e)

    # Return the connection to use
    return connection


################################################################################
# Login Function
#   - This function performs a "SELECT" from the database to check for the
#       student with the same unikey and password as given.
#   - Note: This is only an exercise, there's much better ways to do this
################################################################################

def check_login(sid, pwd):
    # Ask for the database connection, and get the cursor set up
    conn = database_connect()
    if(conn is None):
        return None
    cur = conn.cursor()

    try:
        # Try executing the SQL and get from the database
        int(sid)
        str(pwd)
        sql = """SELECT *
                 FROM unidb.student
                 WHERE studid=%s AND password=%s"""
        cur.execute(sql, (sid, pwd))
        r = cur.fetchone()              # Fetch the first row
        cur.close()                     # Close the cursor
        conn.close()                    # Close the connection to the db
        return r
    except:
        # If there were any errors, return a NULL row printing an error to the debug
        print("Error Invalid Login")
    cur.close()                     # Close the cursor
    conn.close()                    # Close the connection to the db
    return None



################################################################################
# Get classroom function
#   Include all functions required by the assignment specsv
################################################################################

def get_classroom():

    conn = database_connect()
    if(conn is None):
        return None
    # Sets up the rows as a dictionary
    cur = conn.cursor()
    val = None
    try:
        # Try getting all the information returned from the query
        # NOTE: column ordering is IMPORTANT
        cur.execute("""SELECT classroomid,seats,type
                        FROM unidb.classroom
                        ORDER BY classroomid""")
        val = cur.fetchall()
    except:
        # If there were any errors, we print something nice and return a NULL value
        print("Error fetching from database")

    cur.close()                     # Close the cursor
    conn.close()                    # Close the connection to the db
    return val

def search_classroom(seats):
    
    conn = database_connect()

    # Sets up the rows as a dictionary
    cur = conn.cursor()
    val = None
    try:
        int(seats)
        # Try getting all the information returned from the query
        # NOTE: column ordering is IMPORTANT
 
        cur.execute(f"""SELECT classroomid,seats,type
                        FROM unidb.classroom
                        WHERE seats >= '{seats}'
                        ORDER BY classroomid""")
        val = cur.fetchall()
    except:
        # If there were any errors, we print something nice and return a NULL value
        print("Error fetching from database")

    cur.close()                     # Close the cursor
    conn.close()                    # Close the connection to the db
    return val
    
def classroom_type():
    
    conn = database_connect()
    if(conn is None):
        return None
    # Sets up the rows as a dictionary
    cur = conn.cursor()
    val = None
    try:
        # Try getting all the information returned from the query
        # NOTE: column ordering is IMPORTANT
        cur.execute("""SELECT type,count(*)
                        FROM unidb.classroom
                        GROUP BY type
                        ORDER BY count(*)""")
        val = cur.fetchall()
    except:
        # If there were any errors, we print something nice and return a NULL value
        print("Error fetching from database")

    cur.close()                     # Close the cursor
    conn.close()                    # Close the connection to the db
    return val

def add_classroom(classroomid,seats,type):
    
    conn = database_connect()
    if(conn is None):
        return None
    # Sets up the rows as a dictionary
    cur = conn.cursor()
    val = None
    try:
        int(classroomid)
        int(seats)
        str(type)
        # Try getting all the information returned from the query
        # NOTE: column ordering is IMPORTANT
        sql = f"""INSERT INTO unidb.classroom VALUES ('{classroomid}','{seats}','{type}');"""
        
        cur.execute(sql)
        conn.commit()
        val = cur.fetchone()
    except:
        # If there were any errors, we print something nice and return a NULL value
        print("Unexpected error adding a classroom")

    cur.close()                     # Close the cursor
    conn.close()                    # Close the connection to the db
    return val

################################################################################
# Assessment Page
################################################################################
    

def get_assessments():

    conn = database_connect()
    if(conn is None):
        return None
    # Sets up the rows as a dictionary
    cur = conn.cursor()
    val = None
    try:
        # Try getting all the information returned from the query
        # NOTE: column ordering is IMPORTANT
        cur.execute("""SELECT *
                        FROM unidb.assessment""")
        val = cur.fetchall()
    except:
        # If there were any errors, we print something nice and return a NULL value
        print("Error fetching from database")

    cur.close()                     # Close the cursor
    conn.close()                    # Close the connection to the db
    return val
    
def search_assessments(units):
    
    conn = database_connect()

    # Sets up the rows as a dictionary
    cur = conn.cursor()
    val = None
    try:
        
        # Try getting all the information returned from the query
        # NOTE: column ordering is IMPORTANT
 
        cur.execute(f"""SELECT uoSCode,ASS_ID,weighting
                        FROM unidb.assessment
                        WHERE uoSCode = '{units}'
                        ORDER BY ASS_ID""")
        val = cur.fetchall()
    except:
        # If there were any errors, we print something nice and return a NULL value
        print("Error fetching from database")

    cur.close()                     # Close the cursor
    conn.close()                    # Close the connection to the db
    return val


def add_assessments(ASS_ID,uosCode,Semester,year,weighting):
    
    conn = database_connect()
    if(conn is None):
        return None
    # Sets up the rows as a dictionary
    cur = conn.cursor()
    val = None
    try:

        # Try getting all the information returned from the query
        # NOTE: column ordering is IMPORTANT
        sql = f"""INSERT INTO unidb.Assessment VALUES ({ASS_ID},'{uosCode}','{Semester}',{year},{weighting});"""
#         sql = f"""INSERT INTO unidb.Assessment VALUES (9,'INFO2005', 'S2', 2004,77);"""
#         sql = f"""INSERT INTO unidb.Assessment VALUES (11,ISYS3333,S2,2004,88);"""
        
        cur.execute(sql)
        conn.commit()
        val = cur.fetchone()
    except:
        # If there were any errors, we print something nice and return a NULL value
        print("Unexpected error adding a assessment")

    cur.close()                     # Close the cursor
    conn.close()                    # Close the connection to the db
    return val

################################################################################
# Update password and address page
################################################################################

def Update_Address(new_address,sid):
    
    conn = database_connect()
    if(conn is None):
        return None
    # Sets up the rows as a dictionary
    cur = conn.cursor()
    val = None
    try:

        # Try getting all the information returned from the query
        # NOTE: column ordering is IMPORTANT
        sql = f"""UPDATE unidb.student SET Address = '{new_address}' WHERE studid = '{sid}';"""
        
        cur.execute(sql)
        conn.commit()
        val = cur.fetchone()
    except:
        # If there were any errors, we print something nice and return a NULL value
        print("Unexpected error updating address")

    cur.close()                     # Close the cursor
    conn.close()                    # Close the connection to the db
    return val

def Update_Password(new_password,sid):
    
    conn = database_connect()
    if(conn is None):
        return None
    # Sets up the rows as a dictionary
    cur = conn.cursor()
    val = None
    try:

        # Try getting all the information returned from the query
        # NOTE: column ordering is IMPORTANT
        sql = f"""UPDATE unidb.Student SET password = '{new_password}' WHERE studid = '{sid}' ;"""
        
        cur.execute(sql)
        conn.commit()
        val = cur.fetchone()
    except:
        # If there were any errors, we print something nice and return a NULL value
        print("Unexpected error updating password")

    cur.close()                     # Close the cursor
    conn.close()                    # Close the connection to the db
    return val




#####################################################
#  Python code if you run it on it's own as 2tier
#####################################################


if (__name__ == '__main__'):
    create_assessments()
    print("{}\n{}\n{}".format("=" * 50, "Welcome to the 2-Tier Python Database", "=" * 50))
    print("""
This file is to interact directly with the database.
We're using the unidb (make sure it's in your database)

Try to execute some functions:
check_login('3070799133', 'random_password')
check_login('3070088592', 'Green')
list_units()""")


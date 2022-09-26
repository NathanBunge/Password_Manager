
import ctypes
import os
from tkinter import *
from tkinter import messagebox
  
# locating the 'libsample.so' file in the
# same directory as this file
_file = './libsha.so'
_path = os.path.join(*(os.path.split(__file__)[:-1] + (_file, )))
_mod = ctypes.cdll.LoadLibrary(_path)

class Node:
   def __init__(self):
      self.username = ""
      self.password = ""
      self.nextval = None

run = _mod.run
run.argtypes = (ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p)
run.restype = (ctypes.c_int)




import tkinter as tk

root=tk.Tk()

# setting the windows size
root.geometry("600x400")
root.title("Password Manager")

# declaring string variable
# for storing name and password
name_var=tk.StringVar()
passw_var=tk.StringVar()

result = 0

# login function
def LoginBtn():

   name=name_var.get()
   password=passw_var.get()
	
   print("The name is : " + name)
   print("The password is : " + password)

   if " " in name:
      messagebox.showwarning("", "Username cannot have spaces")
      return

   result = run(1, name.encode(), password.encode())
	
   name_var.set("")
   passw_var.set("")

   print("Result: ", result)

   if result == 1:
      messagebox.showinfo("", "Login Success!")
   if result == 2:
      messagebox.showerror("", "Wrong Password")
   if result == 3:
      messagebox.showerror("", "No account under that Username")


#new accoutn button
def NewBtn():
   name=name_var.get()
   password=passw_var.get()
	
   print("The name is : " + name)
   print("The password is : " + password)
   
   result = run(2, name.encode(), password.encode())

   name_var.set("")
   passw_var.set("")

   print("Result: ", result)

   if result == 4:
      messagebox.showinfo("", "New Account Created")
   if result == 5:
      messagebox.showerror("", "Sorry, an account with the name already exists")

# creating a label for
# name using widget Label
name_label = tk.Label(root, text = 'Username', font=('calibre',10, 'bold'))

# creating a entry for input
# name using widget Entry
name_entry = tk.Entry(root,textvariable = name_var, font=('calibre',10,'normal'))

# creating a label for password
passw_label = tk.Label(root, text = 'Password', font = ('calibre',10,'bold'))

# creating a entry for password
passw_entry=tk.Entry(root, textvariable = passw_var, font = ('calibre',10,'normal'), show = '*')

# creating a button using the widget
# Button that will call the submit function
log_btn=tk.Button(root,text = 'Login', command = LoginBtn)

# creating a button using the widget
# Button that will call the submit function
new_btn=tk.Button(root,text = 'New Account', command = NewBtn)

# placing the label and entry in
# the required position using grid
# method
name_label.grid(row=1,column=1)
name_entry.grid(row=1,column=2)
passw_label.grid(row=2,column=1)
passw_entry.grid(row=2,column=2)
log_btn.grid(row=3,column=2)
new_btn.grid(row=4, column=2)
root.grid_columnconfigure(0, weight=1)
root.grid_columnconfigure(3, weight=1)
root.grid_rowconfigure(5, weight=1)






# performing an infinite loop
# for the window to display
root.mainloop()

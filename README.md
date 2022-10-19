## Password Manager Program


## Purpose: 
Passwords are necessary to protect user data in today’s age. When verifying a user logging in, the input password must be compared with the password stored under that user’s file. However, passwords cannot just be stored in text file on a computer. This would defeat the purpose of passwords because an unknown user could find the password and then gain access to another user’s private data. This means passwords need to be stored in a secure way so that they can be checked when logging in, but in a way that they will not be readable in pain text. They also need to be store in a way to that an attacker could not “work backwards” to figure out passwords. This means similar passwords must be indistinguishable from each other.

## Goal
The project will consist of a program that is able to store passwords securely and access/compare passwords when logging in.

## Features
- The program will use a secure algorithm that will hash passwords so that that they are unidentifiable to itself and similar passwords, and then put
them into storage.
- When logging in, the program will compare username and passwords to the corresponding stored username and password. It will do this by hashing the
input password to the stored password and comparing them. 
- The program will have a GUI for users to be able to both register accounts and log into accounts

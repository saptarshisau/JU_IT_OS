#Team-Number-05, Section- A1
#Name- Saptarshi Sau, Roll- 002411001031
#Name- Ankit Roy, Roll- 002411001028

#!/bin/bash

# ==============================================================
# HELPER FUNCTION
#
# Pause after every question.
# Previous output remains visible on the screen.
# ==============================================================

pause()
{
    echo
    read -p "Press Enter to continue to the next question..."
    echo
}


# ==============================================================
# START
# ==============================================================

echo "=============================================================="
echo "              LINUX SHELL LAB DEMONSTRATION"
echo "=============================================================="
echo
echo "This script demonstrates all 14 questions."
echo "Press Enter to begin."
read
echo


# ==============================================================
# QUESTION 1
#
# Find all files that are more than 100 bytes in size and display
# the number of characters, words and lines in each file.
# ==============================================================

echo "=============================================================="
echo "QUESTION 1"
echo "=============================================================="
echo "Find all files that are more than 100 bytes in size and display"
echo "the number of characters, words and lines in each file."
echo "=============================================================="

# Create a file larger than 100 bytes for testing.
echo "This is a test file containing more than one hundred bytes. This file is created automatically for Question 1." > q1_large.txt

# Create a file smaller than 100 bytes for comparison.
echo "Small file" > q1_small.txt

echo
echo "Files larger than 100 bytes:"
echo "--------------------------------------------------------------"

# -type f      -> search only regular files
# -size +100c  -> files greater than 100 bytes
# wc -m        -> number of characters
# wc -w        -> number of words
# wc -l        -> number of lines
find . -type f -size +100c -exec wc -m -w -l {} +

pause


# ==============================================================
# QUESTION 2
#
# Change the modification time to current time for those files
# that have been modified 5 minutes ago.
# ==============================================================

echo "=============================================================="
echo "QUESTION 2"
echo "=============================================================="
echo "Change the modification time to current time for those files"
echo "that have been modified 5 minutes ago."
echo "=============================================================="

# Create a test file.
echo "This file is created for Question 2." > q2_five_minutes_old.txt

# Make its modification time approximately 5 minutes old.
touch -d "5 minutes ago" q2_five_minutes_old.txt

echo
echo "Modification time BEFORE:"
echo "--------------------------------------------------------------"
ls -l q2_five_minutes_old.txt

# Find files modified 5 minutes ago and update their timestamp.
find . -type f -mmin 5 -exec touch {} \;

echo
echo "Modification time AFTER:"
echo "--------------------------------------------------------------"
ls -l q2_five_minutes_old.txt

pause


# ==============================================================
# QUESTION 3
#
# Remove the read permission for others for all .c files in your
# home directory and subdirectories.
#
# The teacher will execute the script from a new directory.
# Therefore, we search only the CURRENT DIRECTORY and its
# subdirectories.
# ==============================================================

echo "=============================================================="
echo "QUESTION 3"
echo "=============================================================="
echo "Remove the read permission for others for all .c files in your"
echo "home directory and subdirectories."
echo "=============================================================="

# Create C files for testing.
echo '#include <stdio.h>' > q3_test1.c
echo 'int main() { return 0; }' >> q3_test1.c

echo '#include <stdio.h>' > q3_test2.c
echo 'int main() { printf("Hello"); return 0; }' >> q3_test2.c

# Give others read permission initially.
chmod 644 q3_test1.c
chmod 644 q3_test2.c

echo
echo "Permissions BEFORE:"
echo "--------------------------------------------------------------"
ls -l q3_test1.c q3_test2.c

# Search current directory and all subdirectories.
# Remove read permission for others.
find . -type f -name "*.c" -exec chmod o-r {} +

echo
echo "Permissions AFTER:"
echo "--------------------------------------------------------------"
ls -l q3_test1.c q3_test2.c

echo
echo "Verification:"
echo "--------------------------------------------------------------"

# Verify that others' read permission has been removed.
for file in $(find . -type f -name "*.c")
do
    permission=$(ls -l "$file" | cut -c10)

    if [ "$permission" = "-" ]
    then
        echo "$file : Others read permission removed successfully."
    else
        echo "$file : Others read permission still exists."
    fi
done

pause


# ==============================================================
# QUESTION 4
#
# You want to make default permission for directories to be
# rwxr--r--. Set your umask variable accordingly. Create some
# directories to see whether it is working or not.
# ==============================================================

echo "=============================================================="
echo "QUESTION 4"
echo "=============================================================="
echo "You want to make default permission for directories to be"
echo "rwxr--r--. Set your umask variable accordingly. Create some"
echo "directories to see whether it is working or not."
echo "=============================================================="

# Normal default permission for directories is 777.
# Required permission is 744.
# Therefore:
# 777 - 744 = 033
# Hence umask = 033.

old_umask=$(umask)

# Set required umask.
umask 033

# Remove old test directories.
rm -rf q4_dir1 q4_dir2

# Create directories using the new umask.
mkdir q4_dir1 q4_dir2

echo
echo "Permissions of newly created directories:"
echo "--------------------------------------------------------------"
ls -ld q4_dir1 q4_dir2

# Restore the original umask.
umask "$old_umask"

pause


# ==============================================================
# QUESTION 5
#
# Create a hard link 'copy' of a file in the current directory.
# Verify the hard link and use the hard-linked file to access
# the same contents.
# ==============================================================

echo "=============================================================="
echo "QUESTION 5"
echo "=============================================================="
echo "Create a hard link 'copy' of a file in the current directory."
echo "Verify the hard link and use the hard-linked file to access"
echo "the same contents."
echo "=============================================================="

# Remove old test files if they exist.
rm -f q5_original.txt copy

# Create an original file.
echo "This is the original file created for testing hard links." > q5_original.txt

# Create a hard link named copy.
ln q5_original.txt copy

echo
echo "Original file and hard link:"
echo "--------------------------------------------------------------"

# -i displays inode number.
# Both names should have the same inode number.
ls -li q5_original.txt copy

echo
echo "Checking inode numbers:"
echo "--------------------------------------------------------------"

inode1=$(ls -li q5_original.txt | awk '{print $1}')
inode2=$(ls -li copy | awk '{print $1}')

if [ "$inode1" = "$inode2" ]
then
    echo "Hard link verified: both files have the same inode."
else
    echo "Hard link verification failed."
fi

echo
echo "Contents using the hard link:"
echo "--------------------------------------------------------------"
cat copy

echo
echo "Modifying the original file:"
echo "--------------------------------------------------------------"

# Modify the original file.
echo "This line was added through the original file." >> q5_original.txt

# Both names refer to the same inode,
# therefore the change is visible through copy.
echo "Contents using copy after modifying original:"
echo "--------------------------------------------------------------"
cat copy

pause


# ==============================================================
# QUESTION 6
#
# Suppose you have a C program (say add.c) that takes the name
# of a person and telephone number as command line arguments and
# appends this information to a file 'telDir.dat'. Consider
# 'add.o' is the object file created after compilation. Ask one
# of your friends to execute this file and examine the result.
# Set the User ID bit for this file and ask your friend to execute
# this file again and examine the result.
#
# First user:
# Saptarshi Sau - 9830013866
#
# Second user:
# Ankit Roy - 9433481289
#
# IMPORTANT:
# Replace ankit_user with Ankit's ACTUAL Linux username.
# Ask Ankit to run: whoami
# ==============================================================

echo "=============================================================="
echo "QUESTION 6"
echo "=============================================================="
echo "Suppose you have a C program (say add.c) that takes the name"
echo "of a person and telephone number as command line arguments and"
echo "appends this information to a file 'telDir.dat'. Consider"
echo "'add.o' is the object file created after compilation. Ask one"
echo "of your friends to execute this file and examine the result."
echo "Set the User ID bit for this file and ask your friend to execute"
echo "this file again and examine the result."
echo "=============================================================="

# Replace this with Ankit's real Linux login username.
ankit_user="REPLACE_WITH_ANKIT_USERNAME"

# Create the C source file.
cat > add.c << 'EOF'
#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *fp;

    if (argc != 3)
    {
        printf("Usage: %s <name> <phone>\n", argv[0]);
        return 1;
    }

    fp = fopen("telDir.dat", "a");

    if (fp == NULL)
    {
        perror("Cannot open telDir.dat");
        return 1;
    }

    fprintf(fp, "%s | %s\n", argv[1], argv[2]);

    fclose(fp);

    printf("Record added successfully.\n");

    return 0;
}
EOF

# Compile source file into object file.
gcc -c add.c -o add.o

# Link object file to create executable.
gcc add.o -o add

# Create data file.
touch telDir.dat

# 644 = rw-r--r--
# Owner has read/write.
# Group and others have read only.
chmod 644 telDir.dat

# Give normal executable permission.
chmod 755 add

echo
echo "Before setting SUID:"
echo "--------------------------------------------------------------"
ls -l add telDir.dat

echo
echo "Saptarshi Sau executes the program:"
echo "--------------------------------------------------------------"

# Saptarshi owns the file, so writing succeeds.
./add "Saptarshi Sau" "9830013866"

echo
echo "Contents after Saptarshi executes:"
echo "--------------------------------------------------------------"
cat telDir.dat

echo
echo "Ankit Roy executes the program BEFORE SUID:"
echo "--------------------------------------------------------------"

# Ankit does not have write permission on telDir.dat.
if sudo -u "$ankit_user" ./add "Ankit Roy" "9433481289"
then
    echo "Unexpected result: Ankit was able to write before SUID."
else
    echo "Expected result: Ankit could not write before SUID."
fi

echo
echo "Setting SUID bit:"
echo "--------------------------------------------------------------"

# Set User ID bit.
chmod u+s add

# The 's' in the owner's execute position shows SUID.
ls -l add

echo
echo "Ankit Roy executes the program AFTER SUID:"
echo "--------------------------------------------------------------"

# Real UID      = Ankit
# Effective UID = owner of add
#
# Therefore the program uses the owner's permissions for
# accessing telDir.dat.
if sudo -u "$ankit_user" ./add "Ankit Roy" "9433481289"
then
    echo "Expected result: Ankit successfully appended the record."
else
    echo "SUID execution failed."
    echo "Check Ankit's actual Linux username."
fi

echo
echo "Final contents of telDir.dat:"
echo "--------------------------------------------------------------"
cat telDir.dat

pause


# ==============================================================
# QUESTION 7
#
# A shared directory is to be created for project group.
# Implement this using sticky bit.
# ==============================================================

echo "=============================================================="
echo "QUESTION 7"
echo "=============================================================="
echo "A shared directory is to be created for project group."
echo "Implement this using sticky bit."
echo "=============================================================="

# Remove old directory if it exists.
rm -rf project_shared

# Create shared directory.
mkdir project_shared

# 1 -> Sticky bit
# 7 -> owner rwx
# 7 -> group rwx
# 7 -> others rwx
chmod 1777 project_shared

echo
echo "Permissions of shared directory:"
echo "--------------------------------------------------------------"
ls -ld project_shared

pause


# ==============================================================
# QUESTION 8
#
# Copy /etc/passwd file in your home directory as password.
# Substitute the word 'bash' by the word 'sh' using vi editor.
#
# Since the script runs from a new directory, the copy is created
# in the current directory.
# ==============================================================

echo "=============================================================="
echo "QUESTION 8"
echo "=============================================================="
echo "Copy /etc/passwd file in your home directory as password."
echo "Substitute the word 'bash' by the word 'sh' using vi editor."
echo "=============================================================="

# Copy /etc/passwd to password.
cp /etc/passwd password

# Replace bash with sh using vi.
# % -> entire file
# s -> substitute
# g -> all occurrences on each line
vi -c '%s/bash/sh/g' -c 'wq' password

echo
echo "First five lines after bash -> sh:"
echo "--------------------------------------------------------------"
head -n 5 password

pause


# ==============================================================
# QUESTION 9
#
# Each line in password file contains several fields separated
# by ':'. Substitute all occurrences of ':' by '|' using vi editor.
# ==============================================================

echo "=============================================================="
echo "QUESTION 9"
echo "=============================================================="
echo "Each line in password file contains several fields separated"
echo "by ':'. Substitute all occurrences of ':' by '|' using vi editor."
echo "=============================================================="

# Replace every ':' with '|'.
vi -c '%s/:/|/g' -c 'wq' password

echo
echo "First five lines after ':' -> '|':"
echo "--------------------------------------------------------------"
head -n 5 password

pause


# ==============================================================
# QUESTION 10
#
# Substitute all occurrences of the word 'home' from line number
# 100 to 200 of 'password' file using vi editor.
# ==============================================================

echo "=============================================================="
echo "QUESTION 10"
echo "=============================================================="
echo "Substitute all occurrences of the word 'home' from line number"
echo "100 to 200 of 'password' file using vi editor."
echo "=============================================================="

# 100,200 -> only lines 100 through 200
# s         -> substitute
# home      -> search word
# house     -> replacement
# g         -> all occurrences on each line
vi -c '100,200s/home/house/g' -c 'wq' password

echo
echo "Lines 100 through 200 have been processed."

pause


# ==============================================================
# QUESTION 11
#
# Delete all lines from a file containing the word 'unix'.
# ==============================================================

echo "=============================================================="
echo "QUESTION 11"
echo "=============================================================="
echo "Delete all lines from a file containing the word 'unix'."
echo "=============================================================="

# /etc/passwd may not contain the word unix.
# Therefore create a separate test file automatically.
echo "hello world" > q11_testfile.txt
echo "unix is an operating system" >> q11_testfile.txt
echo "this line should remain" >> q11_testfile.txt
echo "I use unix commands" >> q11_testfile.txt
echo "another normal line" >> q11_testfile.txt

echo
echo "Contents BEFORE deleting lines containing unix:"
echo "--------------------------------------------------------------"
cat q11_testfile.txt

# Delete all lines containing unix.
# g       -> global
# /unix/  -> search for unix
# d       -> delete line
vi -c 'g/unix/d' -c 'wq' q11_testfile.txt

echo
echo "Contents AFTER deleting lines containing unix:"
echo "--------------------------------------------------------------"
cat q11_testfile.txt

pause


# ==============================================================
# QUESTION 12
#
# Write a shell script, which gets executed the moment the user
# opens a terminal. It should display the message
# "Good morning"/"Good Afternoon"/"Good evening" depending upon
# the time.
#
# In an actual Linux terminal, this logic could be placed in
# .bashrc. Here the logic is executed directly.
# ==============================================================

echo "=============================================================="
echo "QUESTION 12"
echo "=============================================================="
echo "Write a shell script, which gets executed the moment the user"
echo "opens a terminal. It should display the message"
echo '"Good morning"/"Good Afternoon"/"Good evening" depending upon'
echo "the time."
echo "=============================================================="

# Get current hour in 24-hour format.
current_hour=$(date +%H)

# 00 to 11 -> morning
if [ "$current_hour" -lt 12 ]
then
    echo "Good morning!"

# 12 to 16 -> afternoon
elif [ "$current_hour" -lt 17 ]
then
    echo "Good Afternoon!"

# 17 to 23 -> evening
else
    echo "Good evening!"
fi

pause


# ==============================================================
# QUESTION 13
#
# The file /etc/passwd contains information about all the users.
# Write a shell script which would receive the login name during
# execution, obtain information about it from /etc/passwd and
# display the information on the screen in easily understandable
# format.
#
# If a username is supplied while executing this combined script,
# it is used. Otherwise the current user is used.
# ==============================================================

echo "=============================================================="
echo "QUESTION 13"
echo "=============================================================="
echo "The file /etc/passwd contains information about all the users."
echo "Write a shell script which would receive the login name during"
echo "execution, obtain information about it from /etc/passwd and"
echo "display the information on the screen in easily understandable"
echo "format."
echo "=============================================================="

# First command-line argument is the login name.
# If no argument is given, use the current user.
target_user="${1:-$USER}"

echo
echo "Searching information for user: $target_user"
echo "--------------------------------------------------------------"

# Find the line belonging to the user.
user_record=$(grep "^${target_user}:" /etc/passwd)

# Check whether user_record is empty.
if [ -z "$user_record" ]
then
    echo "User '$target_user' does not exist."
else

    # /etc/passwd fields are separated by ':'.
    IFS=: read -r uname pwd uid gid fullname homedir shell_path <<< "$user_record"

    # Display information in readable format.
    echo "Username       : $uname"
    echo "User ID (UID)  : $uid"
    echo "Group ID (GID) : $gid"
    echo "Full Name      : $fullname"
    echo "Home Directory : $homedir"
    echo "Login Shell    : $shell_path"
fi

pause


# ==============================================================
# QUESTION 14
#
# Write a shell script, which receives two filenames as arguments.
# It should check whether the two file's contents are same or not
# (Files may have arbitrary blank lines and the command "cmp"
# would not work in this case). If they are same one would be
# deleted.
# ==============================================================

echo "=============================================================="
echo "QUESTION 14"
echo "=============================================================="
echo "Write a shell script, which receives two filenames as arguments."
echo "It should check whether the two file's contents are same or not"
echo "(Files may have arbitrary blank lines and the command 'cmp'"
echo "would not work in this case). If they are same one would be"
echo "deleted."
echo "=============================================================="

# Create first file.
echo "apple" > q14_file1.txt
echo "banana" >> q14_file1.txt
echo "carrot" >> q14_file1.txt

# Create second file with arbitrary blank lines.
echo "apple" > q14_file2.txt
echo "" >> q14_file2.txt
echo "banana" >> q14_file2.txt
echo "" >> q14_file2.txt
echo "carrot" >> q14_file2.txt

echo
echo "File 1:"
echo "--------------------------------------------------------------"
cat -n q14_file1.txt

echo
echo "File 2:"
echo "--------------------------------------------------------------"
cat -n q14_file2.txt

echo
echo "Comparing files while ignoring blank lines:"
echo "--------------------------------------------------------------"

# diff -B -> ignore blank-line differences
# -q    -> only report whether files are different
# /dev/null -> suppress normal output
if diff -q -B q14_file1.txt q14_file2.txt > /dev/null 2>&1
then
    echo "Files are same when blank lines are ignored."

    # Delete the second file.
    rm -f q14_file2.txt

    echo "q14_file2.txt has been deleted."
else
    echo "Files are different."
    echo "No file was deleted."
fi

pause


# ==============================================================
# FINAL CLEANUP
#
# Remove temporary files created by this script.
#
# Keep password so Questions 8-10 can be inspected after execution.
# ==============================================================

echo "=============================================================="
echo "FINAL CLEANUP"
echo "=============================================================="

rm -f q1_large.txt
rm -f q1_small.txt
rm -f q2_five_minutes_old.txt

rm -f q3_test1.c
rm -f q3_test2.c

rm -rf q4_dir1
rm -rf q4_dir2

rm -f q5_original.txt
rm -f copy

rm -f add.c
rm -f add.o
rm -f add
rm -f telDir.dat

rm -rf project_shared

rm -f q11_testfile.txt

rm -f q14_file1.txt
rm -f q14_file2.txt

echo
echo "Temporary test files created by the script have been removed."
echo "The modified file 'password' has been kept."
echo
echo "=============================================================="
echo "              ALL 14 QUESTIONS COMPLETED"
echo "=============================================================="
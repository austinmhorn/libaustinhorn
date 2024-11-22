import os
import subprocess
import sys
from datetime import datetime

# Log file setup
logs_dir = "logs"
os.makedirs(logs_dir, exist_ok=True)
timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
log_file_path = os.path.join(logs_dir, f"run_engine_{timestamp}.txt")

# Redirect all output to the same log file
def log_message(message):
    with open(log_file_path, "a") as log_file:
        log_file.write(f"{message}\n")
    print(message)

# Execute a script and log its output
def execute_script(script, *args):
    command = ["python3", script] + list(args)
    log_message(f"Executing: {' '.join(command)}")
    with open(log_file_path, "a") as log_file:
        process = subprocess.Popen(command, stdout=log_file, stderr=log_file)
        process.wait()
    if process.returncode == 0:
        log_message(f"✅ {script} completed successfully.")
    else:
        log_message(f"❌ {script} failed with return code {process.returncode}.")

# Get the date from the command line argument or default to today's date
if len(sys.argv) == 2:
    try:
        provided_date = sys.argv[1]
        datetime.strptime(provided_date, "%Y.%m.%d")  # Validate date format
        log_message(f"Using provided date: {provided_date}")
    except ValueError:
        log_message("❌ Error: The provided date must be in the format YYYY.MM.DD")
        sys.exit(1)
else:
    provided_date = datetime.now().strftime("%Y.%m.%d")
    log_message(f"No date provided. Defaulting to today's date: {provided_date}")

# Main workflow
if __name__ == "__main__":
    try:
        
        # Step 1: Clean attachments directory
        log_message("Cleaning attachments directory...")
        clean_command = "rm -R attachments/*"
        clean_process = subprocess.run(clean_command, shell=True, capture_output=True, text=True)
        if clean_process.returncode == 0:
            log_message("✅ Attachments directory cleaned successfully.")
        else:
            log_message(f"❌ Error cleaning attachments directory: {clean_process.stderr}")

        # Step 2: Parse Gmail inbox
        execute_script("parse_inbox.py", provided_date, "--log", log_file_path)

        # Step 3: Run C++ program (Weekly-Report-Engine)
        log_message("Executing Weekly-Report-Engine...")
        process = subprocess.Popen(["./build/Weekly-Report-Engine"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        with open(log_file_path, "a") as log_file:
            stdout, stderr = process.communicate()
            log_file.write(stdout.decode())
            log_file.write(stderr.decode())
        if process.returncode == 0:
            log_message("✅ Weekly-Report-Engine completed successfully.")
        else:
            log_message(f"❌ Weekly-Report-Engine failed with return code {process.returncode}.")

        # Step 4: Append data to Google Sheets
        execute_script("append_sheet_injection.py", provided_date, "--log", log_file_path)

    except Exception as e:
        log_message(f"❌ An error occurred: {e}")

//////////////////////////////////////////////////////////////////////////////////////////
//	LOG.cpp
//	functions to output an error log
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#include "log.h"

//bool LOG::Init()
//Initiates log. Dont call, it is in the constructor
bool LOG::Init(char * filename)
{
	//Clear the log file contents
	if((logfile=fopen(filename, "wb"))==NULL)
		return false;

	//Close the file, return success
	fclose (logfile);
	return true;
}


//bool LOG::Shutdown()
//Shuts down log, in the destructor.
bool LOG::Shutdown()
{
	if(logfile)
		fclose(logfile);

	return true;
}


//output newline
void LOG::OutputNewline()
{
	//Open the file for append
	if((logfile=fopen("Error Log.txt", "a+"))==NULL)
		return;

	//Write the newline
	putc('\n', logfile);

	//Close the file
	fclose(logfile);
}


//void LOG::OutputError(char * text,...)
void LOG::OutputError(char * text,...)
{
	va_list arg_list;

	//Initialise varible argument list
	va_start(arg_list, text);

	//Open the file for append
	if((logfile=fopen("Error Log.txt", "a+"))==NULL)
		return;

	//Write the text
	fprintf(logfile, "<!> ");
	vfprintf(logfile, text, arg_list);
	putc('\n', logfile);

	//Also write to the console window
	printf("<!> ");
	vprintf(text, arg_list);
	printf("\n");

	//Close the file
	fclose(logfile);
	va_end(arg_list);
}


void LOG::OutputSuccess(char * text,...)
{
	va_list arg_list;

	//Initialise varible argument list
	va_start(arg_list, text);

	//Open the file for append
	if((logfile=fopen("Error Log.txt", "a+"))==NULL)
		return;

	//Write the text
	fprintf(logfile, "<-> ");
	vfprintf(logfile, text, arg_list);
	putc('\n', logfile);

	//Close the file
	fclose(logfile);
	va_end(arg_list);
}
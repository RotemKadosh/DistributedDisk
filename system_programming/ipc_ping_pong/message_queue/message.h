#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

// structure for message queue 
typedef struct mesg_buffer { 
    long mesg_type; 
    char mesg_text[100]; 
} message_t; 
#endif //_MESSAGE_HPP_
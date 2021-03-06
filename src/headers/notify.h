#include <sys/inotify.h>
#include <unistd.h> //sleep() function
#include <stdbool.h>

/*! @fn static inline bool search_com_device(const char *desired_port,int ttl)
  * @brief This is a part of project witch is resposible for detecting a new serial devices via inotify dir detection
  *
  *  The inotify helps us to check state of any path of the system 
  * @param desired_port the path to the tty device
  * @param ttl (time to live) the time for authorization by ibutton
  * @return true - Device has been found, false - Device hasn`t been found or ttl overflow
  */
static inline bool search_com_device(const char *desired_port,int ttl)
{
	int wd,inotify_fd,timeout = 0;
	inotify_fd = inotify_init1(IN_NONBLOCK);               
	if(inotify_fd == -1)
		return false;
	do{
		wd = inotify_add_watch(inotify_fd, desired_port, IN_MODIFY | IN_MOVE);
		sleep(1);
		timeout++;
		/* time for device connection*/
		if(timeout > ttl)
			return false;
	}while(wd == -1);
 	return true;
}


/*! 
 *@brief The functions to_byte_array & to_byte_array were taken from:    
 *https://stackoverflow.com/a/50085715
 */
static inline uint8_t hex(char ch) {
    uint8_t r = (ch > '9') ? (ch - '7') : (ch - '0');
    return r & 0x0F;
}


/*!
 *@brief to_byte_array - prepare hexadecimal string to byte array for hash calculating 
 *@params[in] in - input hex string  
 *@params[in] in_size - count of symbols
 *@params[in] out - output hex 8 byte array  
 *@return count - number of bytes
 */
static inline int to_byte_array(const char *in, size_t in_size, uint8_t *out) {
    int count = 0;
    if (in_size % 2) {
        while (*in && out) {
            *out = hex(*in++);
            if (!*in)
                return count;
            *out = (*out << 4) | hex(*in++);
            *out++;
            count++;
        }
        return count;
    } else {
        while (*in && out) {
            *out++ = (hex(*in++) << 4) | hex(*in++);
            count++;
        }
        return count;
    }
}
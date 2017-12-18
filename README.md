C++ Web API Template for Linux

Instructions:
 Link in library in processRequest() function after recv'ing key/value
 pairs from http. Linked caller functions should return a string in a
 that will be returned to the web caller.

 Setup your webserver to proxy to localhost:45000 or reconfigure ports
 in program as desired.

 Send request to the api via webserver in the format of query key/value
 pairs. Ie. 'web.ip/api/set?key=data&monkey=banana'

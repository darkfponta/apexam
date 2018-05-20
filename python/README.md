# Python exam

The class PostcardList is implemented in the file PostcardList.py. The implementation was moved to this file to reduce the clutter.

## Implemented functions
The required functions have been implemented:

- **readFile(fname)** clears the local variable *postcard* and calls the function **updateList**
- **updateLists(fname)** this function reads the file named *fname* and appends the read lines to the local variable *_postcards*. Then the function **parsePostcards** is called.
- **writeFile(fname)** writes to file all the postcards in memory replacing any previous content.
- **parsePostcards()** parses all the lines loaded in memory according to the pattern `date:$(DATE); from:$(SENDER); to:$(RECEIVER);`. In this process three dictionaries are filled:
    
  - *_date* is a dictionary of type ($DATE): list of indices of _postcard with the given date
  - *_from* is a dictionary of type ($SENDER): list of indices of _postcard with the given sender
  - *_to* is a dictionary of type ($RECEIVER): list of indices of _postcard with the given receiver

- **updateFile(fname)** appends the current memory content to the given file.

After the process of loading, the class allows the user to query some information inside the loaded postcards:
 - **getPostcardsBySender(sender)** returns a list with the postcards corresponding to the given sender
 - **getPostcardsByReceiver(receiver)** returns a list with all the postcards having the given receiver
 - **getPostcardsByDateRange(date_range)** return a list with all the postcards whose date falls inside the range given by the vector `date_range = (start_date, end_date)` 

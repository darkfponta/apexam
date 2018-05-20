import datetime  # use this module to deal with dates:
                 # https://docs.python.org/3/library/datetime.html
                 
class PostcardList(object):

    _file = ''
    _postcards = []
    _date = {}
    _from = {}
    _to = {}

    def writeFile(self, fname):
        pass

    def readFile(self, fname):
        pass

    def parsePostcards(self):
        pass

    def updateFile(self, fname):
        pass

    def updateLists(self, fname):
        pass

    def getNumberOfPostcards(self):
        pass

    def getPostcardsByDateRange(self,date_range):
        pass

    def getPostcardsBySender(self, sender):
        pass

    def getPostcardsByReceiver(self, receiver):
        pass

    def printAll(self):
        pass

    def populatePostcards(self, fname):
        pass

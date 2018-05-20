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
        res = []
        start = date_range[0].date()
        end = date_range[1].date()

        for date in self._date:
            if date >= start and date <= end:
                for idx in self._date[date]:
                    res.append(self._postcards[idx])
        return res


    def getPostcardsBySender(self, sender):
        res = []
        for who in self._from:
            if sender == who:
                for idx in self._from[who]:
                    res.append(self._postcards[idx])
        return res


    def getPostcardsByReceiver(self, receiver):
        res = []
        for who in self._to:
            if receiver == who:
                for idx in self._to[who]:
                    res.append(self._postcards[idx])
        return res

    def printAll(self):
        pass

    def populatePostcards(self, fname):
        pass

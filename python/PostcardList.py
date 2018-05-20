import datetime  # use this module to deal with dates:
                 # https://docs.python.org/3/library/datetime.html
from random import randrange
                 
class PostcardList(object):

    _file = ''
    _postcards = []
    _date = {}
    _from = {}
    _to = {}

    def writeFile(self, fname):
        with open(fname, "w") as fhandle:
            for postcard in self._postcards:
                fhandle.write(postcard + '\n')

    def readFile(self, fname):
        self._postcards = []
        self.updateLists(fname)

    def parsePostcards(self):
        for i,postcard in enumerate(self._postcards):
            postcard = postcard.replace(" ","").split(";")
            keyDate = postcard[0].split(":")[1]
            keyDate = datetime.datetime.strptime(keyDate,"%Y-%m-%d").date() # just date and no time
            keyFrom = postcard[1].split(":")[1]
            keyTo = postcard[2].split(":")[1]

            self._date.setdefault(keyDate,[]).append(i)
            self._from.setdefault(keyFrom,[]).append(i)
            self._to.setdefault(keyTo,[]).append(i)

    def updateFile(self, fname):
        with open(fname, "a") as fhandle:
            for postcard in self._postcards:
                fhandle.write(postcard)

    def updateLists(self, fname):
        self._date = {}
        self._from = {}
        self._to = {}
        self._file = fname
        with open(self._file, "r") as fhandle:
            for postcard in fhandle:
                self._postcards.append(str(postcard))  
        self.parsePostcards()

    def getNumberOfPostcards(self):
        return len(self._postcards)

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
        print("Postcards: total " + str(len(self._postcards)))
        for key in self._postcards:
            print(key)

        print("\nDates: ")
        for dat in self._date:
            print(str(dat) + ": indices ", end=" ")
            for d in self._date[dat]:
                print(str(d), end=" ")
            print()
            
        print("\nSenders: ")
        for dat in self._from:
            print(str(dat) + ": indices ", end=" ")
            for d in self._from[dat]:
                print(str(d), end=" ")
            print()

        print("\nReceivers: ")
        for dat in self._to:
            print(str(dat) + ": indices ", end=" ")
            for d in self._to[dat]:
                print(str(d), end=" ")
            print()

    def populatePostcards(self, fname):
        self._postcards = []

        names = ['Dolan', 'Daisu', 'Morky', 'Gooby', 'Bogs', 'Dafty', 'Prudo', 'Spoderman']
        n = len(names)
        ngen = randrange(10) + 10
        baseYear = '1990-01-01'
        endYear = '2018-01-01'
        baseTimestamp = time.mktime(datetime.datetime.strptime(baseYear, "%Y-%m-%d").timetuple())
        endTimestamp = time.mktime(datetime.datetime.strptime(endYear, "%Y-%m-%d").timetuple())
        delta = endTimestamp - baseTimestamp

        for i in range(ngen):
            fromName = ''
            toName = ''
            if names != []:
                pos = randrange(n)

                fromName = names[pos]
                temp = names[pos]
                names[pos] = names[n - 1]
                names[n - 1] = temp

                pos = randrange(n - 1)
                toName = names[pos]

                date = randrange(delta)
                date = datetime.datetime.fromtimestamp(baseTimestamp + date).strftime('%Y-%m-%d')

                postcard = "date:" + date + "; from:" + fromName + "; to:" + toName + ";"
                self._postcards.append(postcard)

        self.writeFile(fname)

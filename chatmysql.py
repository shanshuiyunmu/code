import pymysql

class chatMysql(object):
    def __init__(self):
        try:
            self.connect=pymysql.connect(host='127.0.0.1',port=3306,user="root",password="123456",db="virtus_data")
            self.cursor=self.connect.cursor()
        except Exception as info:
            print(info)
    def insert(self,sql):
        try:
            self.cursor.execute(sql)
            self.connect.commit()
        except Exception as info:
            self.connect.rollback()
            print(info)
    def select(self,sql):
        try:
            self.cursor.execute(sql)
            count=self.cursor.rowcount
            return count,self.cursor.fetchall()
        except Exception as info:
            self.connect.rollback()
            print(info)



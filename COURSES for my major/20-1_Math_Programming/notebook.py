class Note(object):
    def __init__(self, contents):  #내용 적을 수 있도록 초깃값으로 내용 설정
        self.contents = contents

    def get_number_of_lines(self):
        return self.contents.count("\n")

    def get_number_of_characters(self):
        return len(self.contents)

    def remove(self):
        self.contents = "삭제된 노트입니다."

    def __str__(self):
        return self.contents


class NoteBook(object):
    def __init__(self, name):
        self.name = name
        self.pages = 0
        self.notes = {} #notes 변수를 딕셔너리형으로 선언/ key로 각 page number 설정, value로 note의 인스턴스값

    def add_note(self, note, page_number=0): #새로운 note를 notebook에 삽입하는 함수
        if len(self.notes.keys()) < 300:  #페이지 넘버가 300이하면 계속 추가 할 수 있음
            if page_number == 0: #사용자가 페이지 넘버를 지정하지 않았을 때
                if self.pages < 301:
                    self.notes[self.pages] = note
                    self.pages += 1 #마지막으로 지정된 노트의 다음 페이지
                else: #맨마지막 페이지가 300이 넘어갈때
                    for i in range(300):
                        if i not in list(self.notes.keys()):
                            self.notes[self.pages] = note #빈 페이지 찾아서 설정
            else: #사용자가 페이지 넘버를 지정했을때
                if page_number not in self.notes.keys():
                    self.notes[page_number] = note
                else:
                    print("해당 페이지에는 이미 노트가 존재합니다.")
        else: #페이지 넘버가 300을 넘어가면 추가하지 못하게 함
            print("더 이상 노트를 추가하지 못합니다.")

    def remove_note(self, page_number):  #특정 페이지 번호에 있는 note를 제거하는 함수
        del self.notes[page_number]

    def get_number_of_all_lines(self): #모든 페이지에 있는 줄 수 return
        result = 0
        for k in self.notes.keys():
            result += self.notes[k].get_number_of_lines()
        return result

    def get_number_of_all_characters(self): #모든 페이지에 있는 characters수 return
        result = 0
        for k in self.notes.keys():
            result += self.notes[k].get_number_of_characters()
        return result

    def get_number_of_all_pages(self): #모든 페이지 수 return
        return len(self.notes.keys())

    def __str__(self):
        return self.name

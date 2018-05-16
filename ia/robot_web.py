import web
import robot

urls = (
  '/(.*)', 'hello'
)
app = web.application(urls, globals())

class hello:
  def GET(self, command):
    print("command :",str(command))
    answer=robot.robot_com(str(command))
    print ("answer ", answer)
    return answer

if __name__ == "__main__":
  app.run()
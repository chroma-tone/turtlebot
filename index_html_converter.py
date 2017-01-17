Import("env")

from subprocess import call

def before_build(source, target, env):
    print "### before_build ####"
    index_html_file = open("src/index_html.h", "w")
    call(["xxd", "-i", "web/index.html"], stdout = index_html_file)

env.AddPreAction("$BUILD_DIR/src/BasicArduinoServer.o", before_build)

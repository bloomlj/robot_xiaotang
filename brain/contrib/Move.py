# -*- coding: utf-8-*-  # 前后移动
import sys
import os
import logging
import wiringpi 

reload(sys)
sys.setdefaultencoding('utf8')

WORDS=["ZOU"]
SLUG="move"


def handle(text, mic, profile, wxbot=None):

    logger = logging.getLogger(__name__)
    # get config
    #dev=profile['move']['dev']
    #serial = wiringpi.serialOpen(dev, 9600)
    dev=profile[SLUG]['dev']
    serial = wiringpi.serialOpen(dev, 9600)
  
    if any(word in text for word in [u"往前",u"向前",u"前进"]):
        wiringpi.serialPuts(serial, "w\n")
        mic.say("好的，我来了")
    elif any(word in text for word in [u"往后",u"后退",u"向后"]):
        wiringpi.serialPuts(serial, "s\n")
        mic.say("好的，我走了")
     
    wiringpi.serialClose(serial) 
    return True


def isValid(text):
    """
        Returns True if the input is related to weather.
        Arguments:
        text -- user-input, typically transcribed speech
    """
    return any(word in text for word in [u"走", u"走动"])
    #return u"走" in text

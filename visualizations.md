#

Patterns are conversations

Striving for maximum entropy

# Voip sampled on 10ms interval should look something like this:

-> . . . . . . . . . . . . . . . 
-< . . . . . . . . . . . . . . .

Ironically the vast majority of this is silence. We actually built silence suppression into voip back in the early days

-> . . . . . . . . . . . . . . . 			   . . . . . . . . . . . . . . .			 
-<                              . . . . . . . . . . . . . . 

But it didnt work well because it clashed with other forms of traffic. Holding the port open


Videoconferencing mashes together both the video and audio stream, to save on ports being open. 

 Most voip does not do silence suppression these days because stopping transmission had a tendency to close the port on you, and not have consistent measurements of latency and jitter relative to other traffic which would grab the link. (I call this riding the TCP sawtooth, in my apnic bufferbloat talk)
Videoconferencing looks similar, except that there are a lot more packets arriving on an interval like that. Despite me winging about it it, leveraging the voip clock on a separate 5 tuple to simultaneously measure the amount of bandwidth available for video frames has not happened yet to my knowledge, they just mash the two together. 

Most voip does not do silence suppression these days because stopping transmission had a tendency to close the port on you, and not have consistent measurements of latency and jitter relative to other traffic which would grab the link. (I call this riding the TCP sawtooth, in my apnic bufferbloat talk)
Videoconferencing looks similar, except that there are a lot more packets arriving on an interval like that. Despite me winging about it it, leveraging the voip clock on a separate 5 tuple to simultaneously measure the amount of bandwidth available for video frames has not happened yet to my knowledge, they just mash the two together. 
dtaht
if you had voip arriving on a steady clock, but sending ten packets for a video frame was lagging, reduce the frame rate, or send smaller frames. Similarly, if video was reliabiy being delivered within the voip clock, you can increase resolution or the video frame rate.
Also, high resolution voip (HQ) can theoretically run at 2.7ms at 96khz using the opus codec, and I am seeing people try to go higher with rawer processing. And even then, there is plenty more room at bottom. A good ole fashioned switched network, to go 6 miles, used to be about 36us, nearly a 1000x better than what we achieve today, with packets. 

{"sent_bytes":263436,"backlog_bytes":77214,"drops":0,"marks":146,"avg_delay_us":4823},
{"sent_bytes":0,"backlog_bytes":0,"drops":0,"marks":0,"avg_delay_us":0},
is also a mite wordy. I totally get that your typical json API has to be this way, but I come from an APL (note the L) background, where I would just be slinging an array around... 
s:[26346,0,77214,0,0,146,4823], but that does involve having a really tight contract between the app and the javascript, and also knowing in advance what data is important, and what is not.
I have, like, in my babycake implementation made avg_delay_us into something that aint an average no mo and destructive read, so the dang sawtooths show up... it used to be an ewma...

  


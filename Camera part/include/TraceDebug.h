#ifndef TRACEDEBUG_H
#define TRACEDEBUG_H

#define DEBUG_SERIE

void InitDebugSerie()
{
#ifdef DEBUG_SERIE
    Serial.begin(115200);
    Serial.setDebugOutput(false);
#endif
}

void TraceDebug(const char *message)
{
#ifdef DEBUG_SERIE
    Serial.println(message);
#endif
}

void TraceDebug(String message)
{
#ifdef DEBUG_SERIE
    Serial.println(message);
#endif
}

#endif
import React, { useState } from 'react'
import { StackActions, NavigationActions } from 'react-navigation'
import { StatusBar,
   ActivityIndicator, 
   AsyncStorage ,
   View,
   StyleSheet,
   Text,
   TextInput,
   TouchableOpacity,
   Linking
} from 'react-native'

import api from '../../Services/api'

export default function Login(props) {
  const [username, setUsername] = useState('')
  const [password, setPassword] = useState('')
  const [loading, setLoading] = useState(false)
  const [errorMessage, setErrorMessage] = useState(null)

  async function saveUser(user) {
    await AsyncStorage.setItem('@ListApp:userToken', JSON.stringify(user))
  }

  async function signIn() {
    if (username.length === 0) return

    setLoading(true)

    try {

      const credentials = {
        email: username,
        pwd: password
      }

      const response = await api.post('/login', credentials)

      const user = response.data

      await saveUser(user)

      const resetAction = StackActions.reset({
        index: 0,
        actions: [NavigationActions.navigate({ routeName: 'App' })],
      })

      setLoading(false)

      props.navigation.dispatch(resetAction)
    } catch (err) {
      console.log(err)

      setLoading(false)
      setErrorMessage('User doesnt exist or wrong password')
    }
  }


  async function Register(){
    const resetAction = StackActions.reset({
      index: 0,
      actions: [NavigationActions.navigate({ routeName: 'SignUp' })],
    })
    props.navigation.dispatch(resetAction)
  }

  return (
    <View style={styles.Container}>
      <StatusBar barStyle="light-content" />

      <Text style={styles.Title}>meTodo</Text>
      <Text style={styles.TextInformation}>
        Your To-do List
      </Text>

      {!!errorMessage && <Text style={styles.Error}>{errorMessage}</Text>}

      <View style={styles.Form}>
        <TextInput
          style={styles.Input}
          autoCapitalize="none"
          autoCorrect={false}
          placeholder="Email"
          underlineColorAndroid="rgba(0, 0, 0, 0)"
          value={username}
          onChangeText={username => setUsername(username)}
        />

        <TextInput
          style={styles.Input}
          autoCapitalize="none"
          autoCorrect={false}
          placeholder="Password"
          underlineColorAndroid="rgba(0, 0, 0, 0)"
          secureTextEntry={true}
          value={password}
          onChangeText={password => setPassword(password)}
        />

        <TouchableOpacity onPress={signIn} style={styles.Button}>
          {loading ? (
            <ActivityIndicator size="small" color="#FFF" />
          ) : (
            <Text style={styles.ButtonText}>Sign In</Text>
          )}
        </TouchableOpacity>
        <Text style={styles.TextInformation}>Dont have an account ?</Text>
        <Text style={styles.LinkText} onPress={Register}>Sign Up</Text>
      </View>

    </View>
  )
}

Login.navigationOptions = () => {
  return {
    headerShown:false
  }
}


const styles = StyleSheet.create({
  Container: {
    display: "flex",
    flex: 1,
    backgroundColor: "grey",
    padding:40,
    justifyContent:"center",
    alignItems:"stretch"
  },
  Title:{
    textAlign:"center",
    color:"#ffffff",
    fontSize:24,
    fontWeight:"bold"
  },
  TextInformation:{
    textAlign:"center",
    marginTop:10,
    fontSize:14,
    color:"#dddddd",
    lineHeight:21
  },
  Error:{
    color:"#e37a7a",
    textAlign:"center",
    marginTop:10
  },
  Form: {
    marginTop:20
  },
  Input:{
    backgroundColor: "#ffffff",
    borderRadius:3,
    height:44,
    marginBottom:10
  },
  Button:{
    backgroundColor:"#7e91da",
    borderRadius:3,
    height:44,
    marginTop:10,
    justifyContent:"center",
    alignItems:"center"
  },
  ButtonText:{
    color:"white",
    fontSize:14,
    fontWeight:"bold"
  },
  LinkText:{
    color:"blue",
    marginTop:10,
    textDecorationLine:"underline",
    textDecorationColor:"blue",
    justifyContent:"center",
    textAlign:"center"   
  }

});

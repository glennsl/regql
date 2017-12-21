type state('a) =
  | Loading
  | Loaded('a)
  | Failed(string);

type action('a) =
  | Result('a)
  | Error(string);

let component = ReasonReact.reducerComponent("Container");

let make = (uri, token, query, children) => {
  ...component,
  initialState: () => Loading,
  reducer: (action, _state) =>
    switch action {
    | Result(result) => ReasonReact.Update(Loaded(result))
    | Error(error) => ReasonReact.Update(Failed(error))
    },
  didMount: ({state, reduce}) => {
    Transport.run(uri, token, query)
    |> Js.Promise.then_(
         (data) => {
           reduce(() => Result(data), ());
           Js.Promise.resolve(state)
         }
       )
    |> ignore;
    ReasonReact.NoUpdate
  },
  render: ({state}) => children[0](state)
};